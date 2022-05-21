/**
 * @file main.c
 * @brief 计算一个文件的大小。
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  FILE *fp = NULL;
  fp       = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  u_int64_t cnt = 0;
  while (fgetc(fp) != EOF) {
    cnt++;
  }

  printf("文件 %s 的大小为 %lu Bytes\n", argv[1], cnt);

  if (fclose(fp) == EOF) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
