/**
 * @file main.c
 * @brief 每秒向文件写入时间戳.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifndef LOG_FILE_NAME
  #define LOG_FILE_NAME "/home/yangdi/time_log"
#endif

#ifndef BUFF_SIZE
  #define BUFF_SIZE (1024)
#endif

int main() {
  FILE *fp;
  fp = fopen(LOG_FILE_NAME, "a+");
  if (!fp) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 若文件已经存在, 读取文件内容, 统计行数
  char buff[BUFF_SIZE];
  u_int cnt = 0;
  while (fgets(buff, BUFF_SIZE - 1, fp) != NULL)
    cnt++;

  // 写入记录
  time_t time_stamp;
  struct tm *tm;
  while (1) {
    time(&time_stamp);           // 获取时间戳
    tm = localtime(&time_stamp); // 转换系统时间为时间结构体
    fprintf(fp, "%u %d-%.2d-%.2d %.2d:%.2d:%.2d\n", ++cnt, tm->tm_year + 1900, tm->tm_mon + 1,
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec); // 写入记录到文件
    fflush(fp); // 除了终端设备默认为行缓冲模式, 其他的默认为全缓冲模式
    sleep(1);   // 暂停 1 秒
  }

  fclose(fp); // 执行不到

  exit(EXIT_SUCCESS);
}
