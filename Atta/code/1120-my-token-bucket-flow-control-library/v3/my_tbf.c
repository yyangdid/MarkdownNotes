#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "my_tbf.h"

// 单个令牌桶的结构体
struct my_tbf__st {
  int cps;   // 每秒的传输的字符个数
  int burst; // 令牌个数上限
  int token; // 当前已使用令牌个数, 1 个令牌对应一个字符, 即: 一个字符就是一个令牌
  int pos;             // 令牌桶在令牌桶数组中的下标
  pthread_mutex_t mut; // 保证 token 的使用不会产生竞争
  pthread_cond_t cond; // 条件变量, 用来通知当前 token 发生变化的, 和 mut 一起使用
};

// 令牌桶数组
static struct my_tbf__st *job[MY_TBF_MAX];
// 互斥量, 保证上面的 job 数组只能被独占, 防止对 job 数组的使用产生竞争
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
// 标志变量
// static int inited              = 0;
// 保存 SIGALRM 原本的动作
// typedef void (*sighandler_t)(int);
// static sighandler_t alarm_handler_save;

static pthread_t tid_alarm;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

/*
 * @brief 从令牌桶数组中找空闲位置
 * @return 可用的下标, -1 为查找失败, 令牌桶数组满了
 * @note 该函数内部应该对互斥量进行加锁, 但是如果调用该函数的语句已经位于临界区, 那就加两次锁了, 所
 * 以该函数的实现没有使用互斥量加锁, 函数名上有体现, 使用该函数前应该有对互斥量加锁的语句.
 */
static int get_free_pos_unlocked(void) {
  for (int i = 0; i < MY_TBF_MAX; i++) {
    if (job[i] == NULL)
      return i;
  }
  return -1;
}

/*
 * @brief SIGALRM 信号要做的动作
 */
static void *thread_alarm(void *s) {
  while (1) {
    pthread_mutex_lock(&mut_job);
    for (int i = 0; i < MY_TBF_MAX; i++) {
      if (job[i] != NULL) {
        pthread_mutex_lock(&job[i]->mut);
        job[i]->token += job[i]->cps;
        if (job[i]->token > job[i]->burst)
          job[i]->token = job[i]->burst;
        pthread_cond_broadcast(&job[i]->cond);
        pthread_mutex_unlock(&job[i]->mut);
      }
    }
    pthread_mutex_unlock(&mut_job);
    sleep(1); // 不要用 sleep(3) 函数, 用其他函数替代, 这里只是为了简单
  }
}

/*
 * @brief 恢复 SIGALRM 的信号, 关闭 alarm(2), 并释放所有令牌桶
 */
static void module_unload(void) {
  pthread_cancel(tid_alarm);
  pthread_join(tid_alarm, NULL);
  for (int i = 0; i < MY_TBF_MAX; i++) {
    if (job[i] != NULL) {
      my_tbf__destroy(job[i]);
    }
  }
  pthread_mutex_destroy(&mut_job); // 销毁互斥量
}

/*
 * @brief 对信号注册行为, 并发出第一个 alarm 信号
 */
static void module_load(void) {
  int thread_err = pthread_create(&tid_alarm, NULL, thread_alarm, NULL);
  if (thread_err) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 钩子函数，程序结束前最后执行
  atexit(module_unload);
}

/*
 * @brief 初始化令牌桶
 * @param cps[in] 每秒钟传输的字符个数
 * @param burst[in] 令牌个数上限
 * @return 令牌桶结构示例的首地址
 */
my_tbf__t *my_tbf__init(int cps, int burst) {
  struct my_tbf__st *me;

  // 确保 signal(2) 和 alarm(2) 只在被初始化时调用一次
  // 因为一个程序中可以存在多个令牌桶, 而一个程序中只能一个 alarm(1) 在运行
  // // lock();
  // if (!inited) {
  //   module_load();
  //   inited = 1;
  // }
  // // unlock();

  pthread_once(&init_once, module_load);

  me = malloc(sizeof(*me));
  if (me == NULL)
    return NULL;
  me->token = 0;
  me->cps   = cps;
  me->burst = burst;
  pthread_mutex_init(&me->mut, NULL);
  pthread_cond_init(&me->cond, NULL);

  pthread_mutex_lock(&mut_job);
  // 从令牌桶数组获取空闲位置
  int pos = get_free_pos_unlocked();
  if (pos < 0) {
    pthread_mutex_unlock(&mut_job);
    free(me);
    return NULL;
  }

  me->pos  = pos;
  job[pos] = me; // 放入令牌桶数组
  pthread_mutex_unlock(&mut_job);

  return me;
}

// 返回两个整数中较小的一个
static int min(int a, int b) {
  if (a < b)
    return a;
  return b;
}

/*
 * @brief 从令牌桶 my_tbf 中取 num 个令牌
 * @param my_tbf[in,out] 令牌桶首地址
 * @param num[in] 令牌个数
 * @return 成功取到的令牌个数
 */
int my_tbf__fetchToken(my_tbf__t *my_tbf, int num) {
  if (num <= 0)
    return -EINVAL;

  struct my_tbf__st *me = my_tbf;

  pthread_mutex_lock(&me->mut);
  while (me->token <= 0) { // 如果令牌桶中没有可用令牌了
    // pthread_mutex_unlock(&me->mut);
    // sched_yield();
    // pthread_mutex_lock(&me->mut);
    pthread_cond_wait(&me->cond, &me->mut);
  }

  int n = min(me->token, num);
  me->token -= n;
  pthread_mutex_unlock(&me->mut);

  return n;
}

/*
 * @brief 向令牌桶 my_tbf 归还 num 个令牌
 * @param my_tbf[in,out] 令牌桶首地址
 * @param num[in] 令牌个数
 * @return 成功归还的令牌个数
 */
int my_tbf__returnToken(my_tbf__t *my_tbf, int num) {
  if (num <= 0)
    return -EINVAL;

  struct my_tbf__st *me = my_tbf;

  pthread_mutex_lock(&me->mut);
  me->token += num;
  if (me->token > me->burst)
    me->token = me->burst;
  pthread_cond_broadcast(&me->cond);
  pthread_mutex_unlock(&me->mut);

  return num;
}

/*
 * @brief 销毁令牌桶
 * @param my_tbf[in,out] 要销毁令牌桶的首地址
 * @return 是否销毁成功, 0 为成功, 非 0 为失败
 */
int my_tbf__destroy(my_tbf__t *my_tbf) {
  struct my_tbf__st *me = my_tbf;
  pthread_mutex_lock(&mut_job);
  job[me->pos] = NULL;
  pthread_mutex_unlock(&mut_job);
  pthread_mutex_destroy(&me->mut);
  pthread_cond_destroy(&me->cond);
  free(my_tbf);

  return 0;
}
