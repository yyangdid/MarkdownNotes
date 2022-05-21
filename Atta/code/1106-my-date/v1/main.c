/**
 * @file main.c
 * @brief 解析命令行参数.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 * @details
 * - `-y` : year 后面必须跟 2 或 4 作为选项
 * - `-m` : month
 * - `-d` : day
 * - `-H` : hour 后面必须跟 12 或 24 作为选项
 * - `-M` : minute
 * - `-S` : second
 */

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef FMT_STR_SIZE
  #define FMT_STR_SIZE (1024)
#endif

#ifndef TIME_STR_SIZE
  #define TIME_STR_SIZE (1024)
#endif

int main(int argc, char *argv[]) {
  // 检查命令行参数
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s [-y 2|4 [-m] [-d] [-H 12|24] [-M] [-S]]\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

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

  // 获取命令行参数, 根据命令行参数设置 strftime(3) 的日期时间格式
  // getopt(3) 获取短格式的参数(只有一个字符), 即: `-X`, 且只获取 `-` 号之后的那个字符(不包括 `-`)
  // getopt(3) 没有获取到参数返回 -1, 获取成功返回获取的参数(char int), 不认识获取的参数返回 ?
  int c;
  char fmt_str[FMT_STR_SIZE];
  fmt_str[0] = '\0';
  FILE *fp   = stdout;
  while (1) {
    // c = getopt(argc, argv, "ymdHMS"); // 只接受传参, 不接受参数选项和非选项传参
    // c = getopt(argc, argv, "y:mdH:MS"); // 接受参数选项, 不接受非选项传参
    // 接受非选项传参(若参数不是以 - 开头, 则认为是非选项传参, getopt(3) 的返回值为 1, 即: c 为 1)
    c = getopt(argc, argv, "-y:mdH:MS");
    if (c < 0) // 获取失败
      break;

    switch (c) {
      // 非选项传参
    case 1:
      if (fp == stdout) { // 若没有打开的文件
        // optind 类似于文件位置指针, 当上面 getopt(3) 获取到第 X 个参数后, optind
        // 已经指向下个参数了 所以这里需要回退一个参数位置
        fp = fopen(argv[optind - 1], "w");
        if (fp == NULL) {
          fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                  strerror(errno));
          fflush(NULL);
          fp = stdout; // 若文件打开失败, 重定向到标准输出
        }
      }
      break;
      // 普通传参
    case 'y': // 选项传参
      if (strcmp(optarg, "2") == 0)
        strncat(fmt_str, "%y ", FMT_STR_SIZE - 1);
      else if (strcmp(optarg, "4") == 0)
        strncat(fmt_str, "%Y ", FMT_STR_SIZE - 1);
      else
        fprintf(stderr, "Invalid argument of -y!\n");
      break;
    case 'm': strncat(fmt_str, "%m ", FMT_STR_SIZE - 1); break;
    case 'd': strncat(fmt_str, "%d ", FMT_STR_SIZE - 1); break;
    case 'H': // 选项传参
      if (strcmp(optarg, "12") == 0)
        strncat(fmt_str, "%I(%P) ", FMT_STR_SIZE - 1);
      else if (strcmp(optarg, "24") == 0)
        strncat(fmt_str, "%H ", FMT_STR_SIZE - 1);
      else
        fprintf(stderr, "Invalid argument of -H!\n");
      break;
    case 'M': strncat(fmt_str, "%M ", FMT_STR_SIZE - 1); break;
    case 'S':
      strncat(fmt_str, "%S ", FMT_STR_SIZE - 1);
      break;
      // 不认识参数
    default: break;
    }
  }

  // 将结构体时间转换为格式化时间
  char time_str[TIME_STR_SIZE] = "";
  strftime(time_str, TIME_STR_SIZE - 1, fmt_str, tm);
  fprintf(fp, "%s\n", time_str);

  if (fp != stdout)
    fclose(fp);

  exit(EXIT_SUCCESS);
}
