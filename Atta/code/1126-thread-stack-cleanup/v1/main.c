/**
 * @file main.c
 * @brief 线程的栈清理.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_func_1(void *p) {
  puts(p);
}

static void *func_1(void *p) {
  printf("Thread is working!\n");

  pthread_cleanup_push(cleanup_func_1, "cleanup: 1");
  pthread_cleanup_push(cleanup_func_1, "cleanup: 2");
  pthread_cleanup_push(cleanup_func_1, "cleanup: 3");

  printf("push over!\n");

  // pthread_cleanup_pop(1); // 3
  // // pthread_cleanup_pop(1); // 2
  // pthread_cleanup_pop(0); // 2
  // // pthread_cleanup_pop(1); // 1
  // pthread_cleanup_pop(0); // 1

  pthread_exit(NULL);

  // 即使放在 pthread_exit(3) 后面也要和 push 一一对应
  // 放在 exit 后面时, 无论 pop 参数是什么, 都按 1 来执行
  pthread_cleanup_pop(1); // 3
  pthread_cleanup_pop(0); // 2
  pthread_cleanup_pop(0); // 1
}

int main() {
  printf("Begin!\n");

  pthread_t tid;
  int terr;
  terr = pthread_create(&tid, NULL, func_1, NULL);
  if (terr) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  pthread_join(tid, NULL);
  printf("End!\n");

  exit(EXIT_SUCCESS);
}
