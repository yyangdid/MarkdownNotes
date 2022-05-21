/**
 * @file main.c
 * @brief 获取一百天以后的日期.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef TIME_STR_SIZE
  #define TIME_STR_SIZE (1024)
#endif

int main() {
  time_t time_stamp;
  time_stamp = time(NULL); // 获取当前系统时间戳
  if (time_stamp == -1) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 将时间戳转换为结构体时间
  struct tm *tm = NULL;
  tm            = localtime(&time_stamp);
  if (!tm) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 将结构体时间转换为格式化时间
  char time_str[TIME_STR_SIZE] = "";
  strftime(time_str, TIME_STR_SIZE, "Now: %Y-%m-%d", tm);
  printf("%s\n", time_str);

  // 加一百天
  tm->tm_mday += 100; // 该值肯定超过 30 了, 没关系
  // mktime(3) 函数会先检查参数是否合法, 若不合法会自动进行调整. 这里可以利用这个副作用
  (void)mktime(tm); // 若不需要一个函数的返回值, 可以在前面添加个 (void)
  strftime(time_str, TIME_STR_SIZE, "100 days later: %Y-%m-%d", tm);
  printf("%s\n", time_str);

  exit(EXIT_SUCCESS);
}
