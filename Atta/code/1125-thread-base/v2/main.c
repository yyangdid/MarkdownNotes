/**
 * @file main.c
 * @brief 线程的基础使用.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *func_1(void *p) {
  printf("Thread is working!\n");
  // return NULL; // 这句不能少, 因为该函数的返回值不是 void, 而是 void*
  // 在线程的退出最好用线程专门的退出函数, 使用该函数以便进行线程的清理, 使用 return 无法做线程清理.
  pthread_exit(NULL);
}

int main() {
  printf("Begin!\n");

  pthread_t tid;
  int terr;
  terr = pthread_create(&tid, NULL, func_1, NULL);
  if (terr) { // 创建线程失败
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(terr));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  printf("End!\n");

  exit(EXIT_SUCCESS);
}
