/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.6
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
  #define THREAD_NUM (RIGHT - LEFT + 1)
#endif

static void *thread_prime(void *p) {
  int i    = *(int *)p;
  int mark = 1;
  for (int j = 2; j < i / 2; j++) {
    if (i % j == 0) {
      mark = 0;
      break;
    }
  }
  if (mark)
    printf("%d is a primer.\n", i);
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THREAD_NUM];

  for (int i = LEFT; i <= RIGHT; i++) { // 创建线程
    int thread_err = pthread_create(tid + (i - LEFT), NULL, thread_prime, &i);
    if (thread_err) {
      // 若出错之前已经创建了一些线程, 这里需要对这些已创建成功的线程进行资源回收
      // 这里为了省事省略了
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  for (int i = LEFT; i <= RIGHT; i++) { // 线程资源回收
    pthread_join(tid[i - LEFT], NULL);
  }

  exit(EXIT_SUCCESS);
}
