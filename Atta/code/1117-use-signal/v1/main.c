/**
 * @file main.c
 * @brief signal(2) 函数的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void int_handler(int s) {
  write(1, "!", 1);
}

int main() {
  // signal(SIGINT, SIG_IGN); // 忽略 SIGINT 信号

  signal(SIGINT, int_handler); // 改变 SIGINT 信号

  for (int i = 0; i < 10; i++) {
    write(1, "*\n", 1);
    sleep(1);
  }

  exit(EXIT_SUCCESS);
}
