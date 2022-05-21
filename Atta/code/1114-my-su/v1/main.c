/**
 * @file main.c
 * @brief 简易 su 命令.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  // 检查命令行参数
  if (argc < 3 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s UID cmd\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid < 0) { // fork(2) 失败
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  if (pid == 0) { // 子进程
    setuid(strtol(argv[1], NULL, 10));
    execvp(argv[2], argv + 2);
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 父进程
  wait(NULL);

  exit(EXIT_SUCCESS);
}
