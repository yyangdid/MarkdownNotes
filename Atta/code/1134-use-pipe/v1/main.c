/**
 * @file main.c
 * @brief 管道通信示例.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define BUFF_SIZE (1024)

int main() {
  // 创建管道
  int pd[2];
  if (pipe(pd) < 0) { // 回填 pd 数组, pd[0] 为读端, pd[1] 端为写端
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 创建子进程
  pid_t pid = fork();
  if (pid < 0) { // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  char buff[BUFF_SIZE];
  if (pid == 0) { // 子进程, 读管道
    close(pd[1]); // 子进程用不到写端, 这里推荐关掉写端
                  // 从读端读取 BUFF_SIZE 个字符 到 buff 中, len 为实际读取数
    ssize_t len = read(pd[0], buff, BUFF_SIZE);
    write(1, buff, len);
    close(pd[0]);              // 用完了读端, 要关掉
  } else {                     // 父进程, 写管道
    close(pd[0]);              // 用不到读端, 关掉
    write(pd[1], "Hello!", 6); // 写管道
    close(pd[1]);              // 用完了写端, 关掉
    wait(NULL);                // 清理子进程资源
    exit(EXIT_SUCCESS);
  }
}
