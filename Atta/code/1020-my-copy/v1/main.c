/**
 * @file main.c
 * @brief 简单的 copy 程序。
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // 检查运行参数
  if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stdout,
            "Wrong number of parameters!\n"
            "Usage: %s src_file des_file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  FILE *fp_src = NULL, *fp_des = NULL;
  int ch;

  // 打开两个文件流
  fp_src = fopen(argv[1], "r");
  if (fp_src == NULL) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  fp_des = fopen(argv[2], "w");
  if (fp_des == NULL) {
    fclose(fp_src); // 前面的 fp_src 已经打开成功了，这里需要关闭，否则就内存泄漏了
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 读取、写入
  while (1) {
    ch = fgetc(fp_src);
    if (ch == EOF)
      break;
    if (fputc(ch, fp_des) == EOF)
      break;
  }

  // 关闭文件流
  // 一般先关闭依赖别人的对象，然后关闭被依赖的对象
  fclose(fp_des);
  fclose(fp_src);

  exit(EXIT_SUCCESS);
}
