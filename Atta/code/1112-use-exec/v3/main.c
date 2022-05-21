/**
 * @file main.c
 * @brief exec(3) 函数族的使用.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main() {
  printf("Begin!\n");

  fflush(NULL);

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  if (pid == 0) { // 子进程
    // execl("/usr/bin/sleep", "sleep", "100", NULL);
    // argv[0] 可以是随意取个名字, 100 也可以用其他方法封装隐藏
    execl("/usr/bin/sleep", "httpd", "100", NULL);
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 父进程
  wait(NULL);

  printf("End!\n");

  exit(EXIT_SUCCESS);
}
