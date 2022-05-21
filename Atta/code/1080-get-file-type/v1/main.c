/**
 * @file main.c
 * @brief 探测一个文件的类型.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int file__getType(const char *file_name) {
  struct stat stat_rest;
  if (stat(file_name, &stat_rest) < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 用系统定义的文件类型宏进行检测
  if (S_ISDIR(stat_rest.st_mode))
    return 'd';
  else if (S_ISCHR(stat_rest.st_mode))
    return 'c';
  else if (S_ISBLK(stat_rest.st_mode))
    return 'b';
  if (S_ISREG(stat_rest.st_mode))
    return '-';
  else if (S_ISLNK(stat_rest.st_mode))
    return 'l';
  else if (S_ISSOCK(stat_rest.st_mode))
    return 's';
  else if (S_ISFIFO(stat_rest.st_mode))
    return 'p';
  else
    return '?';

  // 或使用下面 man 手册给出的写法来判断文件类型(使用位图宏进行检测)
  /*
  switch (stat_rest.st_mode & S_IFMT) {
  case S_IFDIR: return 'd'; break;
  case S_IFCHR: return 'c'; break;
  case S_IFBLK: return 'b'; break;
  case S_IFREG: return '-'; break;
  case S_IFLNK: return 'l'; break;
  case S_IFSOCK: return 's'; break;
  case S_IFIFO: return 'p'; break;
  default: return '?'; break;
  }
  */
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

  printf("%c\n", file__getType(argv[1]));

  exit(EXIT_SUCCESS);
}
