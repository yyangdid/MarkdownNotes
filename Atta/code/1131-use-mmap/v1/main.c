/**
 * @file main.c
 * @brief 内存映射.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // 检查命令行参数
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 用于获取文件大小
  struct stat stat_rest;
  if (fstat(fd, &stat_rest) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 内存映射
  char *str = mmap(NULL, stat_rest.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (str == MAP_FAILED) { // 如果失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  close(fd); // 把内存数据映射到进程空间后, 文件描述符就可以关闭了

  // 计数
  u_int64_t cnt = 0;
  for (int i = 0; i < stat_rest.st_size; i++) {
    if (str[i] == 'a')
      cnt++;
  }

  printf("%lu\n", cnt);

  munmap(str, stat_rest.st_size); // 解除映射

  exit(EXIT_SUCCESS);
}
