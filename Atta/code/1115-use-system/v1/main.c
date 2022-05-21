/**
 * @file main.c
 * @brief system(3) 函数的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  system("date +%s > ~/a0.txt");

  exit(EXIT_SUCCESS);
}
