/**
 * @file main.c
 * @brief 简单的 copy 程序。
 * @version 0.0.5
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef BUFF_SIZE
  #define BUFF_SIZE (1024)
#endif

int main(int argc, char *argv[]) {
  if (argc < 3 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int fd_src, fd_des; // 文件描述符
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

  do {
    fd_des = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd_des < 0) {
      if (errno != EINTR) { // 真错
        close(fd_src);
        fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                strerror(errno));
        fflush(NULL);
        exit(EXIT_FAILURE);
      }
    }
  } while (fd_des < 0);

  char buff[BUFF_SIZE];
  ssize_t len = 0, rtn = 0;
  while (1) {
    len = read(fd_src, buff, BUFF_SIZE);
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

  close(fd_des);
  close(fd_src);

  exit(EXIT_SUCCESS);
}
