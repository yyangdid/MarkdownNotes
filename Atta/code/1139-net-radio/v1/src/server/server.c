#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <net/if.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <unistd.h>

#include "media_lib.h"
#include "proto.h"
#include "server.h"
#include "thread_channel.h"
#include "thread_list.h"

// -------------------------------------------------------------------------------------------------
// 全局变量
// 默认命令行参数
struct server_conf_st server_conf = {.rcv_port  = DEFAULT_RCV_PORT,
                                     .mut_group = DEFAULT_MUT_GROUP,
                                     .media_dir = DEFAULT_MEDIA_DIR,
                                     .run_mode  = RUN_DAEMON,
                                     .if_name   = DEFAULT_IF};

// -------------------------------------------------------------------------------------------------
// 辅助函数
// ----------------------------------------------------------------------------
// 打印帮助信息
static void print_help(void) {
  printf("-g --group        指定多播组 IP.\n"
         "-p --port         指定接收端口.\n"
         "-f --foreground   前台运行.\n"
         "-d --dir          指定媒体库位置.\n"
         "-i --interface    指定网络设备, 即: 指定发送数据包的网卡.\n"
         "-h --help         显示帮助.\n");
}

// ----------------------------------------------------------------------------
// 把当前进程变成一个守护进程
static int daemonize(void) {
  pid_t pid = fork();
  if (pid < 0) { // 失败
    syslog(LOG_ERR, "%s->%d Error: %s", __FILE__, __LINE__, strerror(errno));
    return -1;
  }

  if (pid > 0) // 父进程
    exit(EXIT_SUCCESS);

  // 子进程
  int fd = open("/dev/null", O_RDWR);
  if (fd < 0) {
    syslog(LOG_WARNING, "%s->%d Warning: %s", __FILE__, __LINE__, strerror(errno));
    return -2;
  } else {
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
      close(fd);
  }

  setsid();
  chdir("/"); // 把当前进程的工作路径改变为一个绝对存在的路径
  umask(0);

  return 0;
}

// ----------------------------------------------------------------------------
// 信号退出函数
static void daemon_exit(int s) {
  closelog(); // 关闭系统日志

  exit(EXIT_SUCCESS);
}

// ----------------------------------------------------------------------------
// 初始化 socket
static void socket_init(void) {
  int server_sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_sd < 0) {
    syslog(LOG_ERR, "%s->%d Error: %s", __FILE__, __LINE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  struct ip_mreqn mut_req;
  inet_pton(AF_INET, server_conf.mut_group, &mut_req.imr_multiaddr);
  inet_pton(AF_INET, "0.0.0.0", &mut_req.imr_address);
  mut_req.imr_ifindex = (int)if_nametoindex(server_conf.if_name);
  if (setsockopt(server_sd, IPPROTO_IP, IP_MULTICAST_IF, &mut_req, sizeof(mut_req)) < 0) {
    syslog(LOG_ERR, "%s->%d Error: %s", __FILE__, __LINE__, strerror(errno));
    exit(EXIT_FAILURE);
  }
  // bind() 可省略
};

// -------------------------------------------------------------------------------------------------
// main 函数
int main(int argc, char *argv[]) {
  openlog("net-radio", LOG_PID | LOG_PERROR, LOG_DAEMON); // 打开系统日志

  // 信号退出处理
  struct sigaction sa;
  sa.sa_handler = daemon_exit;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGINT);
  sigaddset(&sa.sa_mask, SIGQUIT);
  sigaddset(&sa.sa_mask, SIGTERM);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);

  // 1 命令行分析
  int opt_index           = 0;
  struct option arg_arr[] = {
      {"group",      1, NULL, 'g'},
      {"port",       1, NULL, 'p'},
      {"foreground", 0, NULL, 'f'},
      {"dir",        1, NULL, 'd'},
      {"interface",  1, NULL, 'i'},
      {"help",       0, NULL, 'h'},
      {NULL,         0, NULL, 0  }
  };
  while (1) {
    int c = getopt_long(argc, argv, "g:p:fd:i:h", arg_arr, &opt_index);
    if (c < 0) // 失败
      break;
    switch (c) {
    case 'g': server_conf.mut_group = optarg; break;
    case 'p': server_conf.rcv_port = optarg; break;
    case 'f': server_conf.run_mode = RUN_FOREGROUND; break;
    case 'd': server_conf.media_dir = optarg; break;
    case 'i': server_conf.if_name = optarg; break;
    case 'h':
      print_help();
      exit(EXIT_SUCCESS);
      break;
    default: abort(); break;
    }
  }

  // 2 把当前进程做成守护进程
  if (server_conf.run_mode == RUN_DAEMON) { // 若后台运行
    if (daemonize() != 0)                   // 失败
      exit(EXIT_FAILURE);
  } else if (server_conf.run_mode == RUN_FOREGROUND) { // 若前台运行
    /* do nothing */
  } else {
    syslog(LOG_ERR, "%s->%d Error: EINVAL, server_conf.run_mode", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  // 3 socket 初始化
  socket_init();

  // 4 获取频道信息
  struct media_lib_listEntry_st *list;
  int list_size;
  int err = media_lib__getChannelList(&list, &list_size);
  if (err) {}

  // 5 创建节目单线程, 该线程专门发送节目单
  thread_list__create(list, list_size);
  /* if error */

  // 6 创建频道线程, 一个频道对应一个线程, 每个线程发送频道数据
  int channel_i;
  for (channel_i = 0; channel_i < list_size; channel_i++) {
    thread_channel__create(list + channel_i);
    /* if error */
  }

  syslog(LOG_DEBUG, "%s->%d Debug: %d channel threads created.", __FILE__, __LINE__, channel_i);

  // 7 程序永不退出
  while (1)
    pause();

  exit(EXIT_SUCCESS);
}
