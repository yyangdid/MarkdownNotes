/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 * @details
 * 不要运行该程序, 在通常机器配置下一定会有资源报警, 内存失败, pid 耗尽之类的.
 * 因为父进程 fork(2) 出第一个子进程, 而这个子进程也会 fork(2) 子进程.
 *
 * 父进程 -> 201 个子进程A
 *   子进程A1 -> 200 个子进程B
 *     子进程B1 -> 199 个子进程C
 *       ......
 *   子进程A2 -> 199 个子进程D
 *     子进程D1 -> 198 个子进程F
 *       ......
 *   ....
 *
 * 总共会 fork(2) 出 201 的阶乘 个进程.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef LEFT
  #define LEFT 30000000
#endif

#ifndef RIGHT
  #define RIGHT 30000200
#endif

int main() {
  for (int i = LEFT, mark; i <= RIGHT; i++) {
    pid = fork();  // 每个 i 使用一个子进程进行运算
    if (pid < 0) { // fork(2) 执行失败
      fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
              strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
    // 父进程只做 fork(2), 判断 i 是否小于等于 RIGHT, i 自增
    if (pid == 0) { // 如果是子进程
      mark = 1;
      for (int j = 2; j < i / 2; j++) {
        if (i % j == 0) {
          mark = 0;
          break;
        }
      }
      if (mark)
        printf("%d is a primer.\n", i);
    }
  }

  exit(EXIT_SUCCESS);
}
