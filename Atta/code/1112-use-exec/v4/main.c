/**
 * @file main.c
 * @brief exec(3) 函数族的使用.
 * @version 0.0.4
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  if (pid == 0) { // 子进程
    execl("/bin/sh", "sh", "-c", "date +%s", NULL);
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 父进程
  wait(NULL);

  exit(EXIT_SUCCESS);
}
