/**
 * @file main.c
 * @brief 简单的 cat 程序实现。
 * @version 0.0.5
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

// 每次读取和写出多少字符
#ifndef CPS
#define CPS (10)
#endif

#ifndef BUFF_SIZE
#define BUFF_SIZE CPS
#endif

static volatile int loop = 0;

static void alarm_handler(int s) {
  // alarm(1);
  loop = 1;
}

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  signal(SIGALRM, alarm_handler);
  // alarm(1);
  struct itimerval itv;
  itv.it_interval.tv_sec = 1;
  itv.it_interval.tv_usec = 0;
  itv.it_value.tv_sec = 1;
  itv.it_value.tv_usec = 0;
  if (setitimer(ITIMER_REAL, &itv, NULL) < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int fd_src, fd_des = 1; // 文件描述符
  do {
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0) {
      if (errno != EINTR) { // 真错
        fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                strerror(errno));
        fflush(NULL);
        exit(EXIT_FAILURE);
      }
    }
  } while (fd_src < 0);

  char buff[BUFF_SIZE];
  ssize_t len = 0, rtn = 0;
  while (1) {
    while (!loop)
      pause();
    loop = 0;
    while ((len = read(fd_src, buff, BUFF_SIZE)) < 0) {
      if (len < 0) {        // 读取出错
        if (errno == EINTR) // 假错
          continue;
        fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                strerror(errno));
        fflush(NULL);
        // 这里没有使用 exit(2)，是因为在这里如果读取出错了需要先关闭 fd_des 和 fd_src，然后
        // 再 exit(2)。这里使用 break 直接跳出循环，后面马上就执行关闭两个 fd 了
        break;
      }
    }
    if (len == 0) // 读取完成
      break;

    ssize_t pos = 0;
    while (len > 0) { // 如果一次写不完，就一直到写完
      rtn = write(fd_des, buff + pos, len);
      if (rtn < 0) {        // 写入失败
        if (errno == EINTR) // 假错
          continue;
        close(fd_des);
        close(fd_src);
        fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                strerror(errno));
        fflush(NULL);
        exit(EXIT_FAILURE);
      }
      pos += rtn;
      len -= rtn;
    }
  }

  close(fd_src);

  exit(EXIT_SUCCESS);
}
