/**
 * @file main.c
 * @brief 线程池类写法.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#ifndef PROCESS_NUM
  #define PROCESS_NUM 20
#endif

#ifndef FILE_NAME
  #define FILE_NAME "/home/yangdi/zz"
#endif

static void func_add(void) {
  FILE *fp;
  fp = fopen(FILE_NAME, "r+");
  if (!fp) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  char line_buff[BUFSIZ];
  int fd = fileno(fp);
  /* if error */

  lockf(fd, F_LOCK, 0);
  fgets(line_buff, BUFSIZ - 1, fp);
  fseek(fp, 0, SEEK_SET);
  // sleep(1); // 测试语句, 放大线程竞争
  fprintf(fp, "%ld\n", strtol(line_buff, NULL, 10) + 1);
  fflush(fp);
  lockf(fd, F_ULOCK, 0);

  fclose(fp);
}

int main() {
  pid_t pid;
  for (int i = 0; i < PROCESS_NUM; i++) {
    pid = fork();
    if (pid < 0) {
      // 失败, 若已经有几个是成功的, 那么这里需要把这些成功的都释放掉, 这里为了示例简单就没写
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
    if (pid == 0) { // 子进程
      func_add();
      exit(EXIT_SUCCESS);
    }
  }

  for (int i = 0; i < PROCESS_NUM; i++)
    wait(NULL);

  exit(EXIT_SUCCESS);
}
