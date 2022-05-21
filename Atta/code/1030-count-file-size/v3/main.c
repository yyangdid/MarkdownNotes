/**
 * @file main.c
 * @brief 计算一个文件的大小.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @brief 获取文件大小.
 * @param[in] file_name 文件名
 * @return 文件大小
 */
static off_t file__getLength(const char *file_name) {
  struct stat stat_rest;

  if (stat(file_name, &stat_rest) < 0) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  return stat_rest.st_size;
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

  printf("%lld\n", (long long)file__getLength(argv[1]));

  exit(EXIT_SUCCESS);
}
