/**
 * @file main.c
 * @brief fork(2) 的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  // 该语句应该由父进程只打印一次
  // 执行 `./main` 没问题, 但是使用重定向输出到文件 `./main > ~/a.txt` 该语句会有两次输出
  // 若要打印的语句中没有 `\n`, 那么执行 `./main` 也会看到该语句打印了两次, `./main > ~/a.txt` 也会
  // 有两次输出
  // 要解决这问题, 需要在 `fork(2)` 之前使用 `fflush(NULL)` 刷新所有成功打开的流
  // 带有 `\n` 的语句在终端上输出正常, 是因为标准终端上的流是行缓冲, 而重定向到文件流是全缓冲
  printf("[%d]: Begin!\n", getpid());
  // printf("[%d]: Begin!", getpid());

  fflush(NULL); // 重要

  pid_t pid;
  pid = fork();
  if (pid < 0) { // 失败
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 下面 4 个打印语句(父子进程各两句) 的顺序是不确定的, 具体由调度器的调度策略来决定哪个进程先运行
  if (pid == 0) { // 子进程
    printf("[%d]: Child is working!\n", getpid());
  } else { // 父进程
    // sleep(1); // 父进程等着, 让子进程先运行. sleep(3) 最好只在测试语句中出现
    printf("[%d]: Parent is working!\n", getpid());
  }

  // 打印两次, 父子进程各打印一次
  printf("[%d]: End!\n", getpid());

  // getchar(); // 暂停程序, 使用 `ps axf` 来观察父子进程的阶梯关系.

  exit(EXIT_SUCCESS);
}
