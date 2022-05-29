/**
 * @file main.c
 * @brief alarm(2) 函数的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  time_t end;
  end = time(NULL) + 5;

  int64_t cnt = 0;

  while (time(NULL) <= end)
    cnt++;

  printf("%ld\n", cnt);

  exit(EXIT_SUCCESS);
}
