/**
 * @file server.c
 * @brief 被动端, 先收后发.
 * @version 0.0.4
 * @author yyangdid\@gmail.com
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "proto.h"

// 最小空闲子进程个数
#define SERVER_MIN_SPARE (5)
// 最大空闲子进程个数
#define SERVER_MAX_SPARE (10)
// 最多同时支持 client 连接数量, 即最多允许子进程的个数
#define CLIENT_MAX       (20)
// 自定义信号
#define SIG_NOTIFY       SIGUSR2
#define LINE_BUFF_SIZE   (80)
#define IP_STR_SIZE      (40)

// socket 全局变量
static int sd;
// 进程池数组指针
static struct server_st *server_poll;
static int idle_count = 0, busy_count = 0;

// 子进程状态
// int reuse // 可以效仿 apache 服务器的做法, 添加一个计数器, 若一个子进程连续执行了 n 次任务, 就杀
// 掉这个子进程, 为的是防止有 bug, 即: 有些 bug 短时间运行影响不大, 但长时间运行这个 bug 可能会带来
// 毁灭性的问题. 所以让一个子进程只运行有限的任务次数或有个时间上限就杀掉该子进程, 该子进程被杀掉后,
// 一些子进程的小 bug 也随之消失. 可以减少踩到 bug 的几率, 防止小 bug 因长时间运行而放大.
enum {
  STATE_IDLE = 0, // 空闲
  STATE_BUSY      // 执行任务
};

struct server_st {
  pid_t pid; // 子进程 id, -1 为未使用, 不存在子进程
  int state; // 子进程状态
};

// 信号处理函数
static void user2_handler(int s) {
  return;
}

// server 子进程工作
static void server_job(int pos) {
  int ppid = getppid(); // 父进程 id
  struct sockaddr_in remote_address;
  socklen_t remote_address_len = sizeof(remote_address);
  int client_sd, len = 0;
  char line_buff[LINE_BUFF_SIZE];
  char ip_str[IP_STR_SIZE];
  while (1) {
    server_poll[pos].state = STATE_IDLE; // 把自己的状态设置为空闲态
    // 给父进程发信号, 让其来查看我的状态. 父进程会循环查看池中所有的子进程
    kill(ppid, SIG_NOTIFY);

    client_sd = accept(sd, (void *)&remote_address, &remote_address_len);
    if (client_sd < 0) {
      if (errno == EINTR || errno == EAGAIN)
        continue;
      else {
        fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
        fflush(NULL);
        exit(EXIT_FAILURE);
      }
    }
    server_poll[pos].state = STATE_BUSY; // 把自己的状态设置为空闲态
    // 给父进程发信号, 让其来查看我的状态. 父进程会循环查看池中所有的子进程
    kill(ppid, SIG_NOTIFY);

    inet_ntop(AF_INET, &remote_address.sin_addr, ip_str, IP_STR_SIZE);
    // 测试语句
    // printf("[%d] client: %s:%d\n", getppid(), ip_str, ntohs(remote_address.sin_port));

    len = snprintf(line_buff, LINE_BUFF_SIZE, FMT_STAMP, (long long)time(NULL));
    if (send(client_sd, line_buff, len, 0) < 0) {
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
    sleep(5); // 测试语句
    close(client_sd);
  }
}

// 向池中添加一个 server 子进程
static int add_1_server(void) {
  if (idle_count + busy_count >= CLIENT_MAX)
    return -1;
  int slot;
  for (slot = 0; slot < CLIENT_MAX; slot++) // 查找池数组中未使用的下标
    if (server_poll[slot].pid == -1)
      break;

  server_poll[slot].state = STATE_IDLE; // 空闲态
  pid_t pid               = fork();
  if (pid < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {     // 子进程
    server_job(slot); // 干活
    exit(EXIT_SUCCESS);
  } else { // 父进程
    server_poll[slot].pid = pid;
    idle_count++;
  }

  return 0;
}

// 从池中删除一个 server 子进程
static int del_1_server(void) {
  if (idle_count == 0)
    return -1;
  for (int i = 0; i < CLIENT_MAX; i++)
    // 若 server 端已启用且处于空闲状态
    if (server_poll[i].pid != -1 && server_poll[i].state == STATE_IDLE) {
      kill(server_poll[i].pid, SIGTERM); // 杀死子进程
      server_poll[i].pid = -1;
      idle_count--;
      break;
    }

  return 0;
}

// 遍历当前池
static int scan_pool(void) {
  // 这里使用了局部变量没有使用全局变量, 是为了防止在进行修改时别人也在用, 先用局部变量进行计数, 最
  // 后在一次赋值给全局变量, 降低产生冲突的几率
  int idle = 0, busy = 0;
  for (int i = 0; i < CLIENT_MAX; i++) {
    if (server_poll[i].pid == -1)
      continue;
    if (kill(server_poll[i].pid, 0)) { // 检测一个进程是否存在. 若不存在, 改变其 pid 为 -1
      server_poll[i].pid = -1;
      continue;
    }
    // 统计进程池的状态
    if (server_poll[i].state == STATE_IDLE) // 空闲
      idle++;
    else if (server_poll[i].state == STATE_BUSY) // 忙
      busy++;
    else { // 既不是空闲也不是忙, 肯定出大错误了
      fprintf(stderr, "unknown state.\n");
      // 既然出了大错误了, 就别 exit() 清理各种数据了, 此时什么都不敢做, 直接 _exit() 或用信号
      // abort() 杀掉当前进程并产生 core dump文件
      // exit(EXIT_FAILURE);
      // _exit(EXIT_FAILURE);
      abort();
    }
  }
  idle_count = idle;
  busy_count = busy;
  return 0;
}

int main(void) {
  struct sigaction sa, old_sa;

  // 下面三句代码就是一个特定的功能: 父进程不需要对子进程收尸了, 子进程结束时它会自行消亡
  // 忽略掉 SIGCHLD 信号的默认行为, 因为父进程的 wait() 收尸就是由该信号进行通知的
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_NOCLDWAIT; // 阻止子进程变成僵尸状态, 免去收尸收僵尸的困扰

  // 对 SIGCHLD 信号注册新行为并保存旧行为
  sigaction(SIGCHLD, &sa, &old_sa);

  sa.sa_handler = user2_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIG_NOTIFY, &sa, &old_sa);

  // 屏蔽信号
  sigset_t set, old_set;
  sigemptyset(&set);
  sigaddset(&set, SIG_NOTIFY);
  sigprocmask(SIG_BLOCK, &set, &old_set);

  // 为进程池数组申请内存
  server_poll = mmap(NULL, sizeof(struct server_st) * CLIENT_MAX, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (server_poll == MAP_FAILED) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 初始化进程池
  for (int i = 0; i < CLIENT_MAX; i++)
    server_poll[i].pid = -1;

  // 1. 获取 SOCKET
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // -- 设置 socket 属性
  int val = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 2. 给 SOCKET 取得地址
  struct sockaddr_in local_address;
  local_address.sin_family = AF_INET;
  local_address.sin_port   = htons(strtol(SERVER_PORT, NULL, 10));
  inet_pton(AF_INET, "0.0.0.0", &local_address.sin_addr);
  if (bind(sd, (void *)&local_address, sizeof(local_address)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 3. 将 SOCKET 置为监听模式
  if (listen(sd, 200) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 创建子进程
  for (int i = 0; i < SERVER_MIN_SPARE; i++)
    add_1_server(); // 向池中添加一个 server 子进程

  // 信号驱动程序
  while (1) {
    sigsuspend(&old_set); // 信号唤醒当前操作
    scan_pool();          // 遍历当前池
    // 管理池  control the poll
    if (idle_count > SERVER_MAX_SPARE) { // 空闲的子进程个数大于空闲上限值
      for (int i = 0; i < (idle_count - SERVER_MAX_SPARE); i++)
        del_1_server();                         // 从池中删除一个 server 子进程
    } else if (idle_count < SERVER_MIN_SPARE) { // 空闲的子进程个数小于空闲下限值
      for (int i = 0; i < (SERVER_MIN_SPARE - idle_count); i++)
        add_1_server();
    }

    // 输出当前池状态
    for (int i = 0; i < CLIENT_MAX; i++) {
      if (server_poll[i].pid == -1)
        putchar('o'); // 子进程不存在
      else if (server_poll[i].state == STATE_IDLE)
        putchar('.'); // 子进程空闲
      else
        putchar('*'); // 子进程工作
    }
    printf("\n");
  }

  sigprocmask(SIG_SETMASK, &old_set, NULL);

  // 4. 接受连接
  // accept();

  // 5. 收/发消息

  // 6. 关闭 SOCKET
  // close(sd);

  exit(EXIT_SUCCESS);
}
