/**
 * @file main.c
 * @brief 管道通信示例.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
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
  if (pid == 0) {   // 子进程, 读管道
    close(pd[1]);   // 子进程用不到写端, 这里推荐关掉写端
    dup2(pd[0], 0); // 重定向管道读端到标准输入
    close(pd[0]);   // 因为已经重定向了, pd[0] 没用了, 可以关掉
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 1); // 将标准输出重定向到黑洞
    dup2(fd, 2); // 将标准出错重定向到黑洞
    execl("/usr/bin/mpg123", "mpg123", "-", NULL);
    // 若能执行到这里就一定是出错了
    perror("execl()");
    exit(EXIT_FAILURE);
  } else {        // 父进程, 写管道
    close(pd[0]); // 用不到读端, 关掉
    // 父进程从网上收数据, 往管道中写(这里需要用到 socket, 还没学, 暂时没法写)

    close(pd[1]); // 用完了写端, 关掉
    wait(NULL);   // 清理子进程资源
    exit(EXIT_SUCCESS);
  }
}
