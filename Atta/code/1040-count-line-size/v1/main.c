/**
 * @file main.c
 * @brief 获取一个文件每行有多少个字符。
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

  // 这两个变量在声明时或 getline(3) 使用之前一定要做初始化
  // 若在 getline(3) 使用之前不做初始化，表面看没问题，实际已经有错误了
  char *line_buff;
  size_t line_size;
  // 可以把下面的初始化语句注释掉看看输出
  line_buff = NULL;
  line_size = 0;

  while (1) {
    if (getline(&line_buff, &line_size, fp) < 0)
      break;
    printf("%lu\n", strlen(line_buff));
    printf("%zu\n", line_size);
  }

  fclose(fp);

  exit(EXIT_SUCCESS);
}
