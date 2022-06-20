#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "tbf.h"

struct tbf_st {
  int cps;   // 速率
  int burst; // 上限
  int token; // 令牌
  int pos;   // 令牌桶在数组中的位置
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

static struct tbf_st *job[TBF_MAX];
static pthread_mutex_t mut_job  = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;
static pthread_t tid;

// -------------------------------------------------------------------------------------------------
// 辅助函数
// ----------------------------------------------------------------------------
// 获取令牌桶数组中的一个空闲下标, 数组已满返回 -1
static int get_freePos_unlocked(void) {
  for (int i = 0; i < TBF_MAX; i++)
    if (job[i] == NULL)
      return i;
  return -1;
}

// ----------------------------------------------------------------------------
// 获取两个整数中较小的一个
static int min(int a, int b) {
  if (a > b)
    return b;
  return a;
}

// ----------------------------------------------------------------------------
// 模块卸载, 对应模块加载
static void module_unload(void) {
  pthread_cancel(tid);
  pthread_join(tid, NULL);

  for (int i = 0; i < TBF_MAX; i++)
    free(job[i]);

  return;
}

// 每秒为令牌桶数组中的每个令牌桶增加令牌
static void *thread_alarm(void *p) {
  while (1) {
    pthread_mutex_lock(&mut_job);
    for (int i = 0; i < TBF_MAX; i++) {
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
    sleep(1);
  }
}

// 模块加载, 每秒增加令牌
static void module_load(void) {
  int err = pthread_create(&tid, NULL, thread_alarm, NULL);
  if (err) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  atexit(module_unload);
}

// -------------------------------------------------------------------------------------------------
// 函数实现
// ----------------------------------------------------------------------------
/*
 * @brief 初始化令牌桶
 * @param cps[in] 传输速率.
 * @param burst[in] 令牌上限.
 * @return 成功初始化令牌桶的地址, 初始化失败返回 NULL.
 */
tbf_t *tbf__init(int cps, int burst) {
  struct tbf_st *me;

  pthread_once(&init_once, module_load);

  me = malloc(sizeof(*me));
  if (me == NULL)
    return NULL;

  me->cps   = cps;
  me->burst = burst;
  me->token = 0;
  pthread_mutex_init(&me->mut, NULL);
  pthread_cond_init(&me->cond, NULL);

  pthread_mutex_lock(&mut_job);
  int pos = get_freePos_unlocked();
  if (pos < 0) {
    pthread_mutex_unlock(&mut_job);
    free(me);
    return NULL;
  }
  me->pos      = pos;
  job[me->pos] = me;
  pthread_mutex_unlock(&mut_job);

  return me;
}

// ----------------------------------------------------------------------------
/*
 * @brief 从令牌桶 tbf 中获取 num 个令牌.
 * @param tbf[in,out] 令牌桶地址.
 * @param num[in] 令牌数量.
 * @return 成功获取的令牌个数.
 */
int tbf__fetchToken(tbf_t *tbf, int num) {
  struct tbf_st *me = tbf;

  pthread_mutex_lock(&me->mut);
  while (me->token <= 0)
    pthread_cond_wait(&me->cond, &me->mut);

  int n = min(me->token, num);
  me->token -= n;
  pthread_mutex_unlock(&me->mut);
  return n;
}

// ----------------------------------------------------------------------------
/*
 * @brief 将 num 个令牌归还到令牌桶 tbf.
 * @param tbf[in,out] 令牌桶地址.
 * @param num[in] 令牌数量.
 * @return 执行状态, 0: 成功.
 */
int tbf__returnToken(tbf_t *tbf, int num) {
  struct tbf_st *me = tbf;

  pthread_mutex_lock(&me->mut);
  me->token += num;
  if (me->token > me->burst)
    me->token = me->burst;
  pthread_cond_broadcast(&me->cond);
  pthread_mutex_unlock(&me->mut);
  return 0;
}

// ----------------------------------------------------------------------------
/*
 * @brief 销毁令牌桶 tbf.
 * @param tbf[in,out] 要销毁的令牌桶.
 * @return 执行状态, 0: 成功.
 */
int tbf__destroy(tbf_t *tbf) {
  struct tbf_st *me = tbf;

  pthread_mutex_lock(&mut_job);
  job[me->pos] = NULL;
  pthread_mutex_unlock(&mut_job);

  pthread_mutex_destroy(&me->mut);
  pthread_cond_destroy(&me->cond);

  free(me);
  return 0;
}
