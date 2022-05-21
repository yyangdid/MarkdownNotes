/**
 * @file main.c
 * @brief 钩子函数的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

static void func_1(void) {
  printf("func_1() is working!\n");
}

static void func_2(void) {
  printf("func_2() is working!\n");
}

static void func_3(void) {
  printf("func_3() is working!\n");
}

int main() {
  printf("Begin!\n");

  atexit(func_1);
  atexit(func_2);
  atexit(func_3);

  printf("End!\n");

  exit(EXIT_SUCCESS);
}
