/**
 * @file main.c
 * @brief stdio 和 sysio 不要混用。
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  putchar('a');     // stdio
  write(1, "b", 1); // sysio

  putchar('a');
  write(1, "b", 1);

  putchar('a');
  write(1, "b", 1);

  exit(EXIT_SUCCESS);
}
