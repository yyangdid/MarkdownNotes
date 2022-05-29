#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_tbf.h"

// 单个令牌桶的结构体
struct my_tbf__st {
  int cps;   // 每秒的传输的字符个数
  int burst; // 令牌个数上限
  int token; // 当前已使用令牌个数, 1 个令牌对应一个字符, 即: 一个字符就是一个令牌
  int pos; // 令牌桶在令牌桶数组中的下标
};

// 令牌桶数组
static struct my_tbf__st *job[MY_TBF_MAX];
// 标志变量
static int inited = 0;
// 保存 SIGALRM 原本的动作
typedef void (*sighandler_t)(int);
static sighandler_t alarm_handler_save;

/*
 * @brief 从令牌桶数组中找空闲位置
 * @return 可用的下标, -1 为查找失败, 令牌桶数组满了
 */
static int get_free_pos(void) {
  for (int i = 0; i < MY_TBF_MAX; i++) {
    if (job[i] == NULL)
      return i;
  }
  return -1;
}

/*
 * @brief SIGALRM 信号要做的动作
 */
static void alarm_handler(int s) {
  alarm(1);
  for (int i = 0; i < MY_TBF_MAX; i++) {
    if (job[i] != NULL) {
      job[i]->token += job[i]->cps;
      if (job[i]->token > job[i]->burst)
        job[i]->token = job[i]->burst;
    }
  }
}

/*
 * @brief 恢复 SIGALRM 的信号, 关闭 alarm(2), 并释放所有令牌桶
 */
static void module_unload(void) {
  signal(SIGALRM, alarm_handler_save);
  alarm(0);
  for (int i = 0; i < MY_TBF_MAX; i++) {
    free(job[i]);
  }
}

/*
 * @brief 对信号注册行为, 并发出第一个 alarm 信号
 */
static void module_load(void) {
  alarm_handler_save = signal(SIGALRM, alarm_handler);
  alarm(1);

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
  if (!inited) {
    module_load();
    inited = 1;
  }

  // 从令牌桶数组获取空闲位置
  int pos = get_free_pos();
  if (pos < 0)
    return NULL;

  me = malloc(sizeof(*me));
  if (me == NULL)
    return NULL;

  me->token = 0;
  me->cps   = cps;
  me->burst = burst;
  me->pos   = pos;
  job[pos]  = me; // 放入令牌桶数组

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

  while (me->token <= 0) // 如果令牌桶中没有可用令牌了(每秒一个信号, 每个信号 10 个令牌)
    pause();

  int n = min(me->token, num);
  me->token -= n;

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
  me->token += num;
  if (me->token > me->burst)
    me->token = me->burst;

  return num;
}

/*
 * @brief 销毁令牌桶
 * @param my_tbf[in,out] 要销毁令牌桶的首地址
 * @return 是否销毁成功, 0 为成功, 非 0 为失败
 */
int my_tbf__destroy(my_tbf__t *my_tbf) {
  struct my_tbf__st *me = my_tbf;
  job[me->pos]          = NULL;
  free(my_tbf);

  return 0;
}
