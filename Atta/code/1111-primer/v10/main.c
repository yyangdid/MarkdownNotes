/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.10
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LEFT
  #define LEFT 30000000
#endif

#ifndef RIGHT
  #define RIGHT 30000200
#endif

#ifndef THREAD_NUM
  #define THREAD_NUM (4)
#endif

static int num = 0; // 任务池中的属性, 0: 无任务, -1: 任务发放完成, >0: 任务存在
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER; // 初始化互斥量
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;  // 初始化条件变量

static void *thread_prime(void *p) {
  // int i    = *(int *)p;
  while (1) { // 线程循环接任务
    pthread_mutex_lock(&mut_num);
    while (num == 0) { // 若任务池没有新任务
      // pthread_mutex_unlock(&mut_num); // 互斥量解锁等待
      // sched_yield();                  // 出让调度器给其他线程
      // pthread_mutex_lock(&mut_num);   // 互斥量加锁
      pthread_cond_wait(&cond_num, &mut_num);
    }
    if (num == -1) {                  // 已经不会有任务了
      pthread_mutex_unlock(&mut_num); // 临界区内出现跳转前一定要解锁互斥量
      break;
    }
    int i = num;
    num   = 0;
    pthread_cond_broadcast(&cond_num);
    pthread_mutex_unlock(&mut_num);

    int mark = 1;
    for (int j = 2; j < i / 2; j++) {
      if (i % j == 0) {
        mark = 0;
        break;
      }
    }
    if (mark)
      printf("[%d]: %d is a primer.\n", (int)p, i);
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THREAD_NUM];

  for (int i = 0; i < THREAD_NUM; i++) { // 创建线程
    int thread_err = pthread_create(tid + i, NULL, thread_prime, (void *)i);
    if (thread_err) {
      // 若出错之前已经创建了一些线程, 这里需要对这些已创建成功的线程进行资源回收
      // 这里为了省事省略了
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  // 下发任务
  for (int i = LEFT; i <= RIGHT; i++) {
    pthread_mutex_lock(&mut_num); // 互斥量加锁
    while (num != 0) {            // 任务池的任务还在
      // pthread_mutex_unlock(&mut_num); // 互斥量解锁, 让线程来抢任务
      // sched_yield();                  // 出让调度器给别的线程
      // pthread_mutex_lock(&mut_num);   // 互斥量加锁
      pthread_cond_wait(&cond_num, &mut_num);
    }
    num = i; // 下发任务
    pthread_cond_signal(&cond_num);
    pthread_mutex_unlock(&mut_num); // 互斥量解锁
  }

  pthread_mutex_lock(&mut_num);
  while (num != 0) { // 若最后一个任务还没有被线程取走
    pthread_mutex_unlock(&mut_num);
    sched_yield();
    pthread_mutex_lock(&mut_num);
  }
  num = -1; // 让抢任务的线程直到任务已经下发完成, 各线程可以退出了
  pthread_cond_broadcast(&cond_num);
  pthread_mutex_unlock(&mut_num);

  for (int i = 0; i < THREAD_NUM; i++) // 线程资源回收
    pthread_join(tid[i], NULL);

  pthread_mutex_destroy(&mut_num); // 销毁互斥量
  pthread_cond_destroy(&cond_num); // 销毁条件变量

  exit(EXIT_SUCCESS);
}
