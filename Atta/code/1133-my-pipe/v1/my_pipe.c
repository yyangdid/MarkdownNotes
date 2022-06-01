#include <pthread.h>

#include "my_pipe.h"

// 管道结构体, 即: 队列结构体
struct my_pipe_st {
  int head; // 头
  int tail; // 尾
  char data[PIPE_SIZE];
  int data_size;       // 当前管道中的有效字节数
  int cnt_reader;      // 读者计数
  int cnd_writer;      // 写者计数
  pthread_mutex_t mut; // 线程互斥量, 对管道的使用是独占的
  pthread_cond_t cond; // 条件变量, 使用通知法
};

/*
 * @brief 初始化管道.
 * @return 成功返回管道的首地址, 失败返回 NULL.
 */
my_pipe_t *my_pipe__init(void) {
  struct my_pipe_st *me;
  me = malloc(sizeof(*me));
  if (!me)
    return NULL;
  me->head       = 0;
  me->tail       = 0;
  me->data_size  = 0;
  me->cnt_reader = 0;
  me->cnd_writer = 0;
  pthread_mutex_init(&me->mut, NULL);
  pthread_cond_init(&me->cond, NULL);
  return me;
}

/*
 * @brief 注册身份, 即: 注册对管道的操作是读者还是写者, 用于管道内计数.
 * @param my_pipe[in, out] 要注册身份到管道的地址.
 * @param opmap[in] 要注册的身份, 位图.
 * @return 是否成功. 0 为成功, 非 0 为失败.
 */
int my_pipe__register(my_pipe_t *my_pipe, int opmap) {
  /* 如果 opmap 位图既不是 1 也不是 2, 就报错 */
  if (!(opmap & MY_PIPE_READER || opmap & MY_PIPE_WRITER))
    return -1;

  struct my_pipe_st *me = my_pipe;

  pthread_mutex_lock(&me->mut); // 防止注册时有注销的
  if (opmap & MY_PIPE_READER)
    me->cnt_reader++;
  if (opmap & MY_PIPE_WRITER)
    me->cnd_writer++;

  pthread_cond_broadcast(&me->cond); // 叫醒其他正在等待的注册者

  // 只有当管道同时有读者和写者时, 管道才能建立成功
  // 若只有读者或只有写者就等待
  while (me->cnt_reader <= 0 || me->cnd_writer <= 0)
    pthread_cond_wait(&me->cond, &me->mut);
  pthread_mutex_unlock(&me->mut);
  return 0;
}

/*
 * @brief 注销身份, 即: 注销对管道的操作是读者还是写者, 用于管道内计数.
 * @param my_pipe[in, out] 要注销身份到管道的地址.
 * @param opmap[in] 要注销的身份, 位图.
 * @return 是否成功. 0 为成功, 非 0 为失败.
 */
int my_pipe__unregister(my_pipe_t *my_pipe, int opmap) {
  /* 如果 opmap 位图既不是 1 也不是 2, 就报错 */
  if (!(opmap & MY_PIPE_READER || opmap & MY_PIPE_WRITER))
    return -1;

  struct my_pipe_st *me = my_pipe;
  pthread_mutex_lock(&me->mut);
  if (opmap & MY_PIPE_READER)
    me->cnt_reader--;
  if (opmap & MY_PIPE_WRITER)
    me->cnd_writer--;

  pthread_cond_broadcast(&me->cond);

  pthread_mutex_unlock(&me->mut);

  return 0;
}

// 读单字节, 从 me 中读取一个字节, 写入到 buff 中
// 返回 0 为成功, 否则为失败
static int my_pipe__readByte_unlock(struct my_pipe_st *me, char *buff) {
  if (me->data_size <= 0)
    return -1;
  *buff    = me->data[me->head];
  me->head = next(me->head); // 队列头移到下一个字节处
  me->data_size--;
  return 0;
}

/*
 * @brief 读管道.
 * @param my_pipe[in,out] 要读取的管道.
 * @param buff[in,out] 存放读取到的内容.
 * @param size[in] 读取多少.
 * @return 实际读取到多少.
 * @note 仿照标准 read(2) 写的.
 */
int my_pipe__read(my_pipe_t *my_pipe, void *buff, size_t size) {
  struct my_pipe_st *me = my_pipe;
  // 锁定管道
  pthread_mutex_lock(&me->mut);

  // 当管道没有数据, 但是有写者才等待
  while (me->data_size <= 0 && me->cnd_writer > 0)
    pthread_cond_wait(&me->cond, &me->mut);        // 解锁, 在临界区外等待
  if (me->data_size <= 0 && me->cnd_writer <= 0) { // 若没有数据且没有写者了, 就退出
    pthread_mutex_unlock(&me->mut);
    return 0;
  }
  int i;
  for (i = 0; i < size; i++) {
    if (my_pipe__readByte_unlock(me, buff + i) != 0) // 若读单字节失败
      break;
  }

  // 通知管道写者可写了
  pthread_cond_broadcast(&me->cond);

  // 解锁管道
  pthread_mutex_unlock(&me->mut);
  return i;
}

/*
 * @brief 写管道.
 * @param my_pipe[in,out] 要写入的管道.
 * @param buff[in] 要写入的内容.
 * @param size[in] 要写入多少.
 * @return 实际写入多少.
 * @note 仿照标准 write(2) 写的.
 */
int my_pipe__write(my_pipe_t *my_pipe, const void *buff, size_t size) {}

/*
 * @brief 销毁管道.
 * @param my_pipe[in,out] 要销毁管道的地址.
 * @return 执行状态, 0 为成功, 只能成功.
 */
int my_pipe__destroy(my_pipe_t *my_pipe) {
  struct my_pipe_st *me = my_pipe;
  pthread_mutex_destroy(&me->mut);
  pthread_cond_destroy(&me->cond);
  free(my_pipe);
  return 0;
}
