/**
 * @file main.c
 * @brief 守护进程.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>
#include <unistd.h>

#ifndef FILE_NAME
  #define FILE_NAME "/home/yangdi/a.txt"
#endif

static int daemonize() {
  pid_t pid = fork();
  if (pid < 0) // 失败
    return -1;
  if (pid > 0) { // 父进程
    exit(EXIT_SUCCESS);
  }

  int fd = open("/dev/null", O_RDWR);
  if (fd < 0) // 失败
    return -1;

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
  // umask(0);

  return 0;
}

int main() {
  openlog("mydaemon", LOG_PID, LOG_DAEMON);

  if (daemonize()) {                        // 实现守护进程, 若失败
    syslog(LOG_ERR, "daemonize() failed!"); // 不要加 \n, 这里 \n 不代表换行, 就是两个普通字符
    exit(EXIT_FAILURE);
  } else {
    syslog(LOG_INFO, "daemonize() successded!");
  }

  // 守护进程任务, 每秒向 `/home/yangdi/a.txt` 递增写入数字
  FILE *fp = fopen(FILE_NAME, "w");
  if (fp == NULL) {
    syslog(LOG_ERR, "fopen() Error: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  syslog(LOG_INFO, "%s was opened.", FILE_NAME);

  for (int i = 0;; i++) {
    fprintf(fp, "%d\n", i);
    fflush(fp);
    syslog(LOG_DEBUG, "%d is printed.", i);
    sleep(1);
  }

  fclose(fp);

  closelog();

  exit(EXIT_SUCCESS);
}
