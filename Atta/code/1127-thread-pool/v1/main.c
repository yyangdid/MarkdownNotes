/**
 * @file main.c
 * @brief 线程池类写法.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef THREAD_NUM
  #define THREAD_NUM 20
#endif

#ifndef FILE_NAME
  #define FILE_NAME "/home/yangdi/zz"
#endif

static void *thread_add(void *p) {
  FILE *fp;
  fp = fopen(FILE_NAME, "r+");
  if (!fp) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  char line_buff[BUFSIZ];
  fgets(line_buff, BUFSIZ - 1, fp);
  fseek(fp, 0, SEEK_SET);
  sleep(1); // 测试语句, 放大线程竞争
  fprintf(fp, "%ld\n", strtol(line_buff, NULL, 10) + 1);
  fclose(fp);
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) {
    int thread_err = pthread_create(tid + i, NULL, thread_add, NULL);
    if (thread_err) {
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(tid[i], NULL);
  }

  exit(EXIT_SUCCESS);
}
