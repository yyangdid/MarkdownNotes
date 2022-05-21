/**
 * @file main.c
 * @brief 使用 dup(2) 和 dup2(2) 重定向 stdout。
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef FILE_NAME
  #define FILE_NAME "/home/yangdi/.trash/out"
#endif

int main(void) {
  int fd;
  fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fd < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 若 fd 不是 1，在这里应该备份原来的 1 以便后面进行恢复，这里暂时不写了

  // 这两句非原子
  // close(1);
  // dup(fd);

  dup2(fd, 1); // 原子操作，相当于上面两句非原子操作

  if (fd != 1) // 如果 fd 本身不是 1 号
    close(fd);

  printf("Hello!\n");

  // 这里应该恢复 1，这里暂时不写了

  exit(EXIT_SUCCESS);
}
