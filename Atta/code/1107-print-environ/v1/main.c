/**
 * @file main.c
 * @brief 输出所有的环境变量.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int main() {
  for (int i = 0; environ[i] != NULL; i++)
    printf("%s\n", environ[i]);

  exit(EXIT_SUCCESS);
}
