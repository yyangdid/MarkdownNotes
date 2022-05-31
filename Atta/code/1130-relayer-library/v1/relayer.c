#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "relayer.h"

// 有限状态机的几种状态
enum {
  STATE_R = 1, // 读取态
  STATE_W,     // 写入态
  STATE_EX,    // 异常处理态
  STATE_T      // 进程结束态
};

// 有限状态机数据结构
struct relayer_fsm_st {
  int state;         // 当前状态机的状态
  int src_fd;        // 源 文件描述符
  int des_fd;        // 目标 文件描述符
  char buff[BUFSIZ]; // 缓冲区
  ssize_t len;       // 读取, 写入多少个字节
  int pos;           // buff 缓冲区游标位置
  char *err_str;     // 出错字符串
  int64_t count;     // 传输了多少字符
};

// 中继引擎任务数据结构, 不是给用户看的, 是给自己看的
struct relayer_job_st {
  int job_state;                      // 任务状态
  int fd1;                            // 参与通信的文件描述符 1
  int fd2;                            // 参与通信的文件描述符 2
  struct relayer_fsm_st fsm12, fsm21; // 两个有限状态机
  int fd1_save, fd2_save;             // 存放文件描述符默认属性
  // struct timeval start, end; // 任务开始时间和结束时间(可选)
};

// 任务数组
static struct relayer_job_st *relayer_job[RELAYER_JOB_MAX];

static pthread_mutex_t mut_relayer_job = PTHREAD_MUTEX_INITIALIZER;

static pthread_once_t init_once = PTHREAD_ONCE_INIT;

// 推动状态机
static void fsm_driver(struct relayer_fsm_st *fsm) {
  ssize_t ret; // 写入返回值

  switch (fsm->state) {
  case STATE_R:                                          // 如果是读取态
    fsm->len = read(fsm->src_fd, fsm->buff, BUFSIZ - 1); // 从 src_fd 读取到 buff 缓冲区
    if (fsm->len == 0)                                   // 读取结束
      fsm->state = STATE_T;                              // 去进程结束态
    else if (fsm->len < 0) {                             // 读取失败
      if (errno == EAGAIN)                               // 读取假错
        fsm->state = STATE_R;                            // 回到读取态
      else {                                             // 读取真错
        fsm->err_str = "read()";                         // 读取出错
        fsm->state   = STATE_EX;                         // 去异常态
      }
    } else {                // 读取完成
      fsm->pos   = 0;       // buff 的游标位置
      fsm->state = STATE_W; // 去写入态
    }
    break;
  case STATE_W:                                               // 如果是写入态
    ret = write(fsm->des_fd, fsm->buff + fsm->pos, fsm->len); // 从 buff 缓冲区拿出数据写入到 des_fd
    if (ret < 0) {                                            // 写入失败
      if (errno == EAGAIN)                                    // 写入假错
        fsm->state = STATE_W;                                 // 回到写入态
      else {                                                  // 写入真错
        fsm->err_str = "write()";                             // 写入出错
        fsm->state   = STATE_EX;                              // 去异常态
      }
    } else {
      fsm->pos += (int)ret; // buff 游标移动到已写入的字节位置
      fsm->len -= ret;
      if (fsm->len == 0)      // 写入完成
        fsm->state = STATE_R; // 去读取态
      else                    // 没写完
        fsm->state = STATE_W; // 回到写入态
    }
    break;
  case STATE_EX:          // 如果是异常态
    perror(fsm->err_str); // 报错(是读取出错还是写入出错)
    fsm->state = STATE_T; // 去进程结束态
    break;
  case STATE_T: // 如果是进程结束态
    /* do something... */
    break;
  default:
    /* do something... */
    // 例如:
    abort();
    break;
  }
}

/*
 * @brief 找到所有处于运行态的非空任务, 推动状态机.
 * @param p 暂时用不到.
 */
static void *thread_reayler(void *p) {
  pthread_mutex_lock(&mut_relayer_job);
  while (1) {
    for (int i = 0; i < RELAYER_JOB_MAX; i++) {
      if (relayer_job[i] != NULL) {
        if (relayer_job[i]->job_state == STATE_RUNNING) {
          fsm_driver(&relayer_job[i]->fsm12); // 推状态机
          fsm_driver(&relayer_job[i]->fsm21); // 推状态机
          if (relayer_job[i]->fsm12.state == STATE_T && relayer_job[i]->fsm21.state == STATE_T)
            relayer_job[i]->job_state = STATE_OVER;
        }
      }
    }
  }
  pthread_mutex_unlock(&mut_relayer_job);
}

// module_unload()

/*
 * @brief 创建一个线程用于推动状态机器.
 */
static void module_load(void) {
  pthread_t tid_relayer;
  int thread_err = pthread_create(&tid_relayer, NULL, thread_reayler, NULL);
  if (thread_err) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
}

/*
 * @brief 获取任务数组中的空位.
 * @return
 * - >=0 : 空位.
 * - < 0 : 无空位.
 */
static int get_freePos_unlocked(void) {
  for (int i = 0; i < RELAYER_JOB_MAX; i++) {
    if (relayer_job[i] == NULL)
      return i;
  }
  return -1;
}

/*
 * @brief 向任务数组中添加任务.
 * @param fd1[in] 文件描述符 1
 * @param fd2[in] 文件描述符 2
 * @return 数组的下标.
 * - >= 0 : 成功, 返回当前任务的 ID.
 * - == -EINVAL : 失败, 参数非法.
 * - == -ENOSPC : 失败, 任务数组已满.
 * - == -ENOMEM : 失败, 内存分配失败.
 */
int relayer__addJob(int fd1, int fd2) {
  struct relayer_job_st *me;

  // 创建一个线程推动状态机, 且该函数为单次调用, 只能第一个人可以调用
  pthread_once(&init_once, module_load);

  me = malloc(sizeof(*me));
  if (!me) // 失败
    return -ENOMEM;
  me->fd1       = fd1;
  me->fd2       = fd2;
  me->job_state = STATE_RUNNING;

  // 确保 fd1 和 fd2 是以非阻塞的方式打开的
  me->fd1_save = fcntl(me->fd1, F_GETFL);
  fcntl(me->fd1, F_SETFL, me->fd1_save | O_NONBLOCK);
  me->fd2_save = fcntl(me->fd2, F_GETFL);
  fcntl(me->fd2, F_SETFL, me->fd2_save | O_NONBLOCK);

  me->fsm12.src_fd = me->fd1;
  me->fsm12.des_fd = me->fd2;
  me->fsm12.state  = STATE_R;

  me->fsm21.src_fd = me->fd2;
  me->fsm21.des_fd = me->fd1;
  me->fsm21.state  = STATE_R;

  pthread_mutex_lock(&mut_relayer_job);     // 加锁
  int pos = get_freePos_unlocked();         // 在数组中找空位
  if (pos < 0) {                            // 失败
    pthread_mutex_unlock(&mut_relayer_job); // 解锁
    fcntl(me->fd1, F_SETFL, me->fd1_save);  // 恢复 fd1 属性
    fcntl(me->fd2, F_SETFL, me->fd2_save);
    free(me); // 释放内存
    return -ENOSPC;
  }
  relayer_job[pos] = me;
  pthread_mutex_unlock(&mut_relayer_job); // 解锁
  return pos;
}

#if 0
/*
 * @brief 取消任务.
 * @param id[in] 要取消任务的 ID.
 * @return 是否成功.
 * - == 0 : 成功, 指定任务成功取消.
 * - == -EINVAL : 失败, 参数非法.
 * - == -EBUSY : 失败, 任务早已被取消.
 */
int relayer__cancelJob(int id);

/*
 * @brief 为指定任务清理资源.
 * @param id[in] 要清理资源的任务 ID.
 * @param relay[in, out] 用于存放任务状态的任务结构体地址.
 * @return 执行结果.
 * - == 0 : 成功, 指定任务已终止并返回状态.
 * - == -EINVAL : 失败, 参数非法.
 */
int relayer__waitJob(int id, struct relayer_stat_st *relay);

/*
 * @brief 获取任务当前的状态.
 * @param id[in] 要获取状态的任务 ID.
 * @param relay[in, out] 用于存放任务状态的任务结构体地址.
 * @return 执行结果.
 * - == 0 : 成功, 指定任务状态已经返回.
 * - == -EINVAL : 失败, 参数非法.
 */
int relayer__statJob(int id, struct relayer_stat_st *relay);
#endif
