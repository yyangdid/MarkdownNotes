/**
 * @file main.c
 * @brief 使用单一计时器, 构造一组函数, 实现任意数量的计时器.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "any_timer.h"

static void func_2(void *p) {
  printf("func_2(): %s\n", p);
}

static void func_1(void *p) {
  printf("func_1(): %s\n", p);
}

/*
 * 先打印 Begin!, 然后打印 End!
 * 然后打印 ..
 * 然后打印 bbb
 * 然后打印 ...
 * 然后打印 aaa
 * 然后打印 ..
 * 然后打印 ccc
 * 然后打印 ...............................
 */
int main() {
  printf("Begin!\n");
  int job1, job2, job3;

  // 5 秒之后调用 func_1(), 参数为 "aaa"
  job1 = any_timer__addJob(5, func_1, "aaa");
  if (job1 < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(-job1));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 2 秒之后调用 func_2(), 参数为 "bbb"
  job2 = any_timer__addJob(2, func_1, "aaa");
  if (job2 < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(-job2));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 7 秒之后调用 func_1(), 参数为 "ccc"
  job3 = any_timer__addJob(7, func_1, "aaa");
  if (job3 < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(-job3));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  printf("End!\n");

  while (1) { // 每秒向终端写入一个字符点 .
    write(1, ".", 1);
    sleep(1);
  }

  exit(EXIT_SUCCESS);
}
