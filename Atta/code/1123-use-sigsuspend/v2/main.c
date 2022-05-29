/**
 * @file main.c
 * @brief sigsuspend(2) 函数的使用.
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void int_handler(int s) {
  write(1, "!", 1);
}

int main() {
  // signal(SIGINT, SIG_IGN); // 忽略 SIGINT 信号

  signal(SIGINT, int_handler); // 改变 SIGINT 信号

  sigset_t set, old_set, save_set; // 定义信号集变量
  sigemptyset(&set);               // 置空信号集变量
  sigaddset(&set, SIGINT);         // 将 SIGINT 信号添加到信号集变量

  // 将信号集内的信号解除阻塞, 并保存解除阻塞之前的状态(即: 调用本函数之前的状态)
  sigprocmask(SIG_UNBLOCK, &set, &save_set);

  sigprocmask(SIG_BLOCK, &set, &old_set); // 阻塞信号集内的信号, 并将阻塞前状态保存到 old_set

  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 5; j++) {
      write(1, "*\n", 1);
      sleep(1);
    }
    write(1, "\n", 1);

    // 下面四句是非原子的, sigsuspend(2) 相当于这四句的原子操作
    // sigset_t tmp_set;                             // 其实 tmp_set 和 set 两个一样
    // sigprocmask(SIG_SETMASK, &old_set, &tmp_set); // 解除阻塞
    // pause();                                      // 等待
    // sigprocmask(SIG_SETMASK, &tmp_set, NULL);     // 恢复 tmp_set, 即: set
    sigsuspend(&old_set);
  }

  sigprocmask(SIG_SETMASK, &save_set, NULL); // 本函数退出之前恢复调用本函数之前的状态

  exit(EXIT_SUCCESS);
}
