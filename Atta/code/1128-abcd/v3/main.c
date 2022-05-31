/**
 * @file main.c
 * @brief 线程池类写法.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef THREAD_NUM
  #define THREAD_NUM (4)
#endif

static int num             = 0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER; // 初始化互斥量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 初始化条件变量

static int next(int n) {
  if (n + 1 == THREAD_NUM)
    return 0;
  return n + 1;
}

static void *thread_func1(void *p) {
  int n = (int)p;
  int c = 'a' + n;
  while (1) {
    pthread_mutex_lock(&mut);
    while (num != n) // 不是我
      pthread_cond_wait(&cond, &mut);
    write(1, &c, 1);
    num = next(num);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mut);
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THREAD_NUM];

  for (int i = 0; i < THREAD_NUM; i++) {
    // // 初始化四个互斥量
    // pthread_mutex_init(mut + i, NULL);
    // // 加锁
    // pthread_mutex_lock(mut + i);
    // 这里先用 (void *)i 这种难看的写法
    int thread_err = pthread_create(tid + i, NULL, thread_func1, (void *)i);
    if (thread_err) {
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  // pthread_mutex_unlock(mut + 0); // 给打印 a 的线程互斥量解锁

  alarm(5); // 暂时使用信号, 5 秒钟后信号杀死进程

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(tid[i], NULL);
  }

  pthread_mutex_destroy(&mut);
  pthread_cond_destroy(&cond);

  exit(EXIT_SUCCESS);
}
