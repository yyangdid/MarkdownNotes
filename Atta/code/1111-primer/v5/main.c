/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.5
 * @author yyangdid\@gmail.com
 * @details
 * 此例子使用了交叉分配法进行子进程的分配, 缺陷在于产生的第一个子进程永远拿不到质数(本例比较特殊),
 * 最好使用任务池的分配方式, 任务池涉及到竞争问题, 这里还没学, 在进程间通信才会学.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#ifndef LEFT
  #define LEFT 30000000
#endif

#ifndef RIGHT
  #define RIGHT 30000200
#endif

// 限制子进程的数量
#ifndef N
  #define N (3)
#endif

int main() {
  pid_t pid;

  for (int n = 0; n < N; n++) {
    pid = fork();
    if (pid < 0) {
      fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
              strerror(errno));
      fflush(NULL);
      // 很可能前两个子进程 fork(2) 成功了, 在第三个失败了, 那么在退出之前需要对前两个进行资源释放
      // 这里应该有个循环把 fork(2) 成功的子进程进行资源释放, 这里暂时省略了
      exit(EXIT_FAILURE);
    }
    if (pid == 0) { // 如果是子进程
      for (int i = LEFT + n, mark; i <= RIGHT; i += N) {
        mark = 1;
        for (int j = 2; j < i / 2; j++) {
          if (i % j == 0) {
            mark = 0;
            break;
          }
        }
        if (mark)
          printf("[%d]: %d is a primer.\n", n, i);
      }
      exit(EXIT_SUCCESS); // 子进程一定要有退出语句
    }
  }

  for (int i = 0; i < N; i++) {
    wait(NULL);
  }

  exit(EXIT_SUCCESS);
}
