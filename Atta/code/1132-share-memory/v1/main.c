/**
 * @file main.c
 * @brief 进程间共享内存的实现.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

#define MEMORY_SIZE (1024)

// 子进程写, 父进程读
int main() {
  // 内存映射, 产生一块匿名内存空间
  char *ptr = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) { // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid < 0) {              // 失败
    munmap(ptr, MEMORY_SIZE); // 取消映射
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  if (pid == 0) { // 子进程, 写
    strcpy(ptr, "Hello");
    munmap(ptr, MEMORY_SIZE); // 子进程退出前先取消映射
    exit(EXIT_SUCCESS);
  } else {      // 父进程, 读
    wait(NULL); // 等着为子进程清理资源
    printf("%s\n", ptr);
    munmap(ptr, MEMORY_SIZE); // 父进程退出前先取消映射
    exit(EXIT_SUCCESS);
  }
}
