/**
 * @file main.c
 * @brief 进程间共享内存的实现.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

#define MEM_SIZE (1024)

int main() {
  int shm_id = shmget(IPC_PRIVATE, MEM_SIZE, 0600); // 创建实例
  if (shm_id < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  char *ptr;
  if (pid == 0) {                 // 子进程, 写
    ptr = shmat(shm_id, NULL, 0); // 映射内存
    if (ptr == (void *)-1) {      // 失败
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
    strcpy(ptr, "Hello!"); // 写
    shmdt(ptr);            // 解除映射
    exit(EXIT_SUCCESS);
  } else {                        // 父进程, 读
    wait(NULL);                   // 等着为子进程是否资源
    ptr = shmat(shm_id, NULL, 0); // 映射内存
    if (ptr == (void *)-1) {      // 失败
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
    printf("%s\n", ptr);
    shmdt(ptr);                     // 解除映射
    shmctl(shm_id, IPC_RMID, NULL); // 父进程销毁实例
    exit(EXIT_SUCCESS);
  }
}
