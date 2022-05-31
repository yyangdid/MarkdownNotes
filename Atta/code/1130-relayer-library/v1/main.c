/**
 * @file main.c
 * @brief 使用有限状态机实现非阻塞 I/O 数据中继在两个 tty 之间传输数据.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "relayer.h"

// 两对设备
#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"

#define TTY3 "/dev/tty11"
#define TTY4 "/dev/tty12"

int main() {
  int fd1, fd2;             // 文件描述符
  fd1 = open(TTY1, O_RDWR); // 以阻塞的读写方式打开 TTY1
  if (fd1 < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  write(fd1, "MY_TTY1\n", 8); // 终端提示语句

  fd2 = open(TTY2, O_RDWR | O_NONBLOCK); // 以非阻塞的读写方式打开 TTY2
  if (fd2 < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  write(fd2, "MY_TTY2\n", 8); // 终端提示语句

  // 添加任务
  int job1 = relayer__addJob(fd1, fd2);
  if (job1 < 0) { // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(-job1));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int fd3, fd4;             // 文件描述符
  fd3 = open(TTY3, O_RDWR); // 以阻塞的读写方式打开 TTY3
  if (fd3 < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  write(fd3, "MY_TTY3\n", 8); // 终端提示语句

  fd4 = open(TTY4, O_RDWR | O_NONBLOCK); // 以非阻塞的读写方式打开 TTY4
  if (fd4 < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  write(fd4, "MY_TTY4\n", 8); // 终端提示语句

  // 添加任务
  int job2 = relayer__addJob(fd3, fd4);
  if (job2 < 0) { // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(-job2));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 让程序一直运行
  while (1)
    pause();

  close(fd2);
  close(fd1);
  close(fd4);
  close(fd3);

  exit(EXIT_SUCCESS);
}
