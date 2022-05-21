/**
 * @file main.c
 * @brief 创建一个空洞文件.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int fd;
  fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fd < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  off_t lseek_ret;
  // 这里的 -1 是为了给下面的 write(2) 写入一个空字符留的空间, 为了好计算
  lseek_ret = lseek(fd, (off_t)5 * 1024 * 1024 * 1024 - 1, SEEK_SET);
  if (lseek_ret < 0) {
    close(fd);
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 这个 write(2) 系统调用, 是为了在文件位置指针出写入字符, 让文件大小能创建成功, 哪怕是空字符
  write(fd, "", 1);

  close(fd);

  exit(EXIT_SUCCESS);
}
