/**
 * @file main.c
 * @brief 筛选质数.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
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
  pid_t pid;
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
      // 把下面这行注释取消掉,把最下面父进程的 sleep(3) 注释掉, 父进程最先退出,
      // 然后使用 `ps axf` 观察这些子进程的层次状态, 所有子进程睡眠态.
      // sleep(1000); // 测试语句
      exit(EXIT_SUCCESS); // 子进程一定要有退出语句
    }
  }
  // 把这行注释取消掉, 也把上面子进程的 sleep(3) 注释取消掉, 观察父子进程的层次状态
  // 父进程为可中断的睡眠态, 所有子进程为僵尸态
  // sleep(1000); // 测试语句

  // 父进程应该对其创建的子进程进行资源回收,
  // 这里还没学到, 暂时没做处理. 默认 init 进程对所有子进程进行资源回收.

  exit(EXIT_SUCCESS);
}
