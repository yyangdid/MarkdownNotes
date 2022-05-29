/**
 * @file main.c
 * @brief alarm(2) 函数的使用.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 使用 gcc 编译时若使用了优化选项, 使用 volatile 关键字, 表示去变量真正的空间去取值
// static int loop = 1;
static volatile int loop = 1;

static void alarm_handler(int s) {
  loop = 0;
}

int main() {
  u_int64_t cnt = 0;
  // signal(2) 一定要写在 alarm(2) 前面
  signal(SIGALRM, alarm_handler);
  alarm(5);

  while (loop)
    cnt++;

  printf("%ld\n", cnt);

  exit(EXIT_SUCCESS);
}
