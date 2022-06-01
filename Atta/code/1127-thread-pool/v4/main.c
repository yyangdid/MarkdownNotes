/**
 * @file main.c
 * @brief 线程池类写法.
 * @version 0.0.4
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

#ifndef PROCESS_NUM
  #define PROCESS_NUM 20
#endif

#ifndef FILE_NAME
  #define FILE_NAME "/home/yangdi/zz"
#endif

int sem_id;

// 取资源量
static void P(void) {
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op  = -1;
  op.sem_flg = 0;
  while (semop(sem_id, &op, 1) < 0) {
    if (errno != EINTR || errno != EAGAIN) {
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }
}

// 归还资源量
static void V(void) {
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op  = 1;
  op.sem_flg = 0;
  if (semop(sem_id, &op, 1) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
}

static void func_add(void) {
  FILE *fp;
  fp = fopen(FILE_NAME, "r+");
  if (!fp) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  char line_buff[BUFSIZ];

  P(); // 取资源量
  fgets(line_buff, BUFSIZ - 1, fp);
  fseek(fp, 0, SEEK_SET);
  // sleep(1); // 测试语句, 放大线程竞争
  fprintf(fp, "%ld\n", strtol(line_buff, NULL, 10) + 1);
  fflush(fp);
  V(); // 归还资源量

  fclose(fp);
}

int main() {
  pid_t pid;
  sem_id = semget(IPC_PRIVATE, 1, 0600); // 创建实例, 匿名 IPC
  if (sem_id < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  if (semctl(sem_id, 0, SETVAL, 1) < 0) { // 初始化实例
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

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

  semctl(sem_id, 0, IPC_RMID); // 销毁实例

  exit(EXIT_SUCCESS);
}
