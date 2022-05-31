/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.11
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "my_semaphore.h"

#ifndef LEFT
  #define LEFT 30000000
#endif

#ifndef RIGHT
  #define RIGHT 30000200
#endif

#ifndef THREAD_NUM
  #define THREAD_NUM (RIGHT - LEFT + 1)
#endif

// 同一时刻最多允许存在的线程数
#ifndef N
  #define N (4)
#endif

static my_sem_t *sem;

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

  sleep(5); // 测试语句, 每个线程要运行 5 秒钟以上, 就可以用 ps ax -L 命令查看进程关系

  my_sem__add(sem, 1);
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THREAD_NUM];

  sem = my_sem__init(N);
  if (!sem) {
    fprintf(stderr, "%s->%d Error\n", __FILE__, __LINE__);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  for (int i = LEFT; i <= RIGHT; i++) { // 创建线程
    my_sem__sub(sem, 1);
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

  my_sem__destroy(sem);

  exit(EXIT_SUCCESS);
}
