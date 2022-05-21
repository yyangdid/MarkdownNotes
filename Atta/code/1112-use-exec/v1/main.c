/**
 * @file main.c
 * @brief exec(3) 函数族的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  printf("Begin!\n");

  fflush(NULL); // 注意

  execl("/usr/bin/date", "date", "+%s", NULL);
  // 如果 exec(3) 函数族执行成功了, 那么已经用新的进程映像替换掉了旧的进程映像, 永远会回来了.
  // 如果回来了, 那么一定是出错了, 所以这里不对 exec(3) 函数族的返回值做判断了, 如果回来一定出错了
  fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
  fflush(NULL);
  exit(EXIT_FAILURE);

  printf("End!\n"); // 不会被执行

  exit(EXIT_SUCCESS);
}
