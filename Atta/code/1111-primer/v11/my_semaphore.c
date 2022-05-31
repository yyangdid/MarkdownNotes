#include <malloc.h>
#include <pthread.h>

#include "my_semaphore.h"

struct my_sem_st {
  int value; // 资源总量值
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

/*
 * @brief 初始化信号量.
 * @param init_value[in] 资源总量值.
 * @return
 */
my_sem_t *my_sem__init(int init_value) {
  struct my_sem_st *me;
  me = malloc(sizeof(*me));
  if (!me)
    return NULL;
  me->value = init_value;
  pthread_mutex_init(&me->mut, NULL);
  pthread_cond_init(&me->cond, NULL);

  return me;
}

/*
 * @brief 归还 num 个资源量到 sem 中.
 * @param sem[in,out] 信号量.
 * @param num[in] 要归还的资源量值.
 * @return 归还资源量的个数
 */
int my_sem__add(my_sem_t *sem, int num) {
  struct my_sem_st *me = sem;
  pthread_mutex_lock(&me->mut);

  me->value += num;
  pthread_cond_broadcast(&me->cond);
  pthread_mutex_unlock(&me->mut);
  return num;
}

/*
 * @brief 从 sem 中取 num 个资源量.
 * @param sem[in,out] 信号量.
 * @param num[in] 希望获取的资源量值.
 * @return 获取资源量的个数
 */
int my_sem__sub(my_sem_t *sem, int num) {
  struct my_sem_st *me = sem;
  pthread_mutex_lock(&me->mut);

  while (me->value < num)
    pthread_cond_wait(&me->cond, &me->mut);

  me->value -= num;

  pthread_mutex_unlock(&me->mut);

  return num;
}

/*
 * @brief 销毁信号量.
 * @param sem[in,out] 要销毁的信号量.
 * @return 销毁状态: 0 为成功
 */
int my_sem__destroy(my_sem_t *sem) {
  struct my_sem_st *me = sem;
  pthread_mutex_destroy(&me->mut);
  pthread_cond_destroy(&me->cond);
  free(me);

  return 0;
}
