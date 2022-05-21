/**
 * @file main.c
 * @brief fopen(3) 函数的基本使用。
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  FILE *fp;
  fp = fopen("tmp", "r");
  if (fp == NULL) {
    // 方法一：
    // 输出：main.c->main()->19 Error: errno = 2
    fprintf(stderr, "%s->%s()->%d Error: errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    // 方法二：
    // 输出：fopen() error: No such file or directory
    perror("fopen() error");
    //方法三：
    // 输出：main.c->main()->25 Error: No such file or directory
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("ok!");

  // 本例中 fopen() 使用了 r 模式打开 tmp 文件，该文件不存在一定会打开失败，不会运行到这里，
  // 需要打开一个存在的文件或使用除了 r 和 r+ 之外的模式打开，即：w w+ a a+
  // fclose() 的返回值一般不需要进行判断，这里严谨了点
  if (fclose(fp) == EOF) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
