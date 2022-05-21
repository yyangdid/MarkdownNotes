/**
 * @file main.c
 * @brief 获取环境变量值.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
  // 此处需对返回值做检查
  printf("%s\n", getenv("PATH"));

  exit(EXIT_SUCCESS);
}
