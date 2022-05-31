/**
 * @file main.c
 * @brief 线程池类写法.
 * @version 0.0.1
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

static void *thread_func1(void *p) {
  int c = 'a' + (int)p;
  while (1)
    write(1, &c, 1);

  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THREAD_NUM];

  for (int i = 0; i < THREAD_NUM; i++) {
    // 这里先用 (void *)i 这种难看的写法
    int thread_err = pthread_create(tid + i, NULL, thread_func1, (void *)i);
    if (thread_err) {
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  alarm(5); // 暂时使用信号, 5 秒钟后信号杀死进程

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(tid[i], NULL);
  }

  exit(EXIT_SUCCESS);
}
