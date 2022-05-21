/**
 * @file main.c
 * @brief 守护进程.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef FILE_NAME
  #define FILE_NAME "/home/yangdi/a.txt"
#endif

static int daemonize() {
  pid_t pid = fork();
  if (pid < 0) {            // 失败
    perror("fork() Error"); //
    return -1;
  }
  if (pid > 0) { // 父进程
    exit(EXIT_SUCCESS);
  }

  int fd = open("/dev/null", O_RDWR);
  if (fd < 0) { // 失败
    perror("open() Error");
    return -1;
  }

  // 子进程
  dup2(fd, 0);
  dup2(fd, 1);
  dup2(fd, 2);

  if (fd > 2)
    close(fd);

  setsid();
  // 改变当前的工作路径, 如果当前工作路径是可卸载设备(例如: U 盘), 因为 daemon 一直在运行,当卸载的时
  // 候会提示设备忙
  chdir("/");
  // 如果确定该程序中不会再产生文件了, 可以把 umask 值关掉 (可选)
  umask(0);

  return 0;
}

int main() {
  if (daemonize()) // 实现守护进程, 若失败
    exit(EXIT_FAILURE);

  // 守护进程任务, 每秒向 `/home/yangdi/a.txt` 递增写入数字
  FILE *fp = fopen(FILE_NAME, "w");
  if (fp == NULL) {
    perror("fopen() Error"); // 守护进程没有终端, 这里为了方便演示, 程序内所有错误暂时这么写
    exit(EXIT_FAILURE);
  }

  for (int i = 0;; i++) {
    fprintf(fp, "%d\n", i);
    fflush(fp);
    sleep(1);
  }

  fclose(fp);

  exit(EXIT_SUCCESS);
}
