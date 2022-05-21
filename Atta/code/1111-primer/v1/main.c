/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef LEFT
  #define LEFT 30000000
#endif

#ifndef RIGHT
  #define RIGHT 30000200
#endif

int main() {
  for (int i = LEFT, mark; i <= RIGHT; i++) {
    mark = 1;
    for (int j = 2; j < i / 2; j++) {
      if (i % j == 0) {
        mark = 0;
        break;
      }
    }
    if (mark)
      printf("%d is a primer.\n", i);
  }

  exit(EXIT_SUCCESS);
}
