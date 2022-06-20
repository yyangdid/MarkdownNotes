/**
 * @file client.c
 * @brief client 端.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 * @details
 * 从 socket 中接收数据, 通过管道将数据传输到子进程中, 由子进程进行播放.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "proto.h"

// -------------------------------------------------------------------------------------------------
// 全局变量
// 默认命令行参数
struct client_conf_st client_conf = {.rcv_port   = DEFAULT_RCV_PORT,
                                     .mut_group  = DEFAULT_MUT_GROUP,
                                     .player_cmd = DEFAULT_PLAYER_CMD};

// -------------------------------------------------------------------------------------------------
// 辅助函数
// ----------------------------------------------------------------------------
// 打印帮助信息
static void print_help(void) {
  printf("-g --group     指定多播组.\n"
         "-p --port      指定接收端口.\n"
         "-P --player    指定播放器命令行.\n"
         "-h --help      显示帮助.\n");
}

/*
 * @brief 向 fd 中写入 buf, 并坚持写够 cnt 个字符.
 * @param fd[in,out] 文件描述符.
 * @param len[in] 要写入的内容.
 * @param cnt[in] 要写入的字符数.
 * @return 成功写入的字符数.
 */
static ssize_t writen(int fd, const void *buf, size_t len) {
  int pos = 0;
  while (len > 0) {
    ssize_t rnt = write(fd, buf + pos, len);
    if (rnt < 0) {
      if (errno == EINTR) // 假错
        continue;
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      return -1;
    }
    pos += rnt;
    len -= rnt;
  }

  return pos;
}

// -------------------------------------------------------------------------------------------------
// main 函数
int main(int argc, char *argv[]) {
  // 初始化级别: 默认值 < 配置文件 < 环境变量 < 命令行参数

  // 1 分析命令行
  // 1-1 通过命令行参数修改默认组播 ip 和端口
  int opt_index           = 0;
  struct option arg_arr[] = {
      {"group",  1, NULL, 'g'},
      {"port",   1, NULL, 'p'},
      {"player", 1, NULL, 'P'},
      {"help",   0, NULL, 'h'},
      {NULL,     0, NULL, 0  }
  };
  while (1) {
    int c = getopt_long(argc, argv, "g:p:P:h", arg_arr, &opt_index);
    if (c < 0) // 失败
      break;
    switch (c) {
    case 'g': client_conf.mut_group = optarg; break;
    case 'p': client_conf.rcv_port = optarg; break;
    case 'P': client_conf.player_cmd = optarg; break;
    case 'h':
      print_help();
      exit(EXIT_SUCCESS);
      break;
    default: abort(); break;
    }
  }

  // 2 socket
  int sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  struct ip_mreqn m_req;
  inet_pton(AF_INET, client_conf.mut_group, &m_req.imr_multiaddr);
  /* 对 inet_pton(3) 返回值进行校验 */
  inet_pton(AF_INET, "0.0.0.0", &m_req.imr_address);
  m_req.imr_ifindex = (int)if_nametoindex("eth0");

  if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &m_req, sizeof(m_req)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int val = 1;
  if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 2-1 bind
  struct sockaddr_in local_address;
  local_address.sin_family = AF_INET;
  local_address.sin_port   = htons(strtol(client_conf.rcv_port, NULL, 10));
  inet_pton(AF_INET, "0.0.0.0", &local_address.sin_addr.s_addr);
  if (bind(sd, (void *)&local_address, sizeof(local_address)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 3 创建匿名管道, 用于向子进程传输数据
  int pd[2];
  if (pipe(pd) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 4 创建子进程, 用于接收管道数据进行播放
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 5 子进程: 调用解码器
  if (pid == 0) {
    close(sd);    // 用不到 socket, 关闭
    close(pd[1]); // 用不到管道的写端, 关闭
    dup2(pd[0], 0); // 将管道的读端重定向到标准输入, 因为解码器从标准输入读取数据
    if (pd[0] > 0) // 若管道的读端文件描述符不是 0, 那么就关闭, 因为重定向到标准输入后就用不到了
      close(pd[0]);

    execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
    // 只有 exec 失败了才会回来执行下面语句
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 6 父进程: 从网络上收包, 发送给子进程
  // 6-1 收节目单
  struct msg_list_st *msg_list; // 节目单包
  msg_list = malloc(MSG_LIST_MAX);
  if (!msg_list) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 6-1-1接收包
  struct sockaddr_in server_address;
  socklen_t server_address_len = sizeof(server_address);
  size_t rcv_len;
  while (1) {
    rcv_len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&server_address, &server_address_len);
    if (rcv_len < sizeof(struct msg_list_st)) { // 没接收到数据
      continue;                                 // 重新尝试接收包
    }
    if (msg_list->channel_id != CHANNEL_LIST_ID) { // 若收到的包不是节目单包, 即: 频道 id 不是 0
      fprintf(stderr, "Channel id is not match.\n"); // 测试用语句, channel id 不对
      continue;
    }
    break; // 接收完成
  }

  // 6-2 打印节目单并选择频道
  struct msg_list_entry_st *pos;
  // 打印节目单
  for (pos = msg_list->list_entry; (char *)pos < ((char *)msg_list) + rcv_len;
       pos = (void *)(((char *)pos) + ntohs(pos->len))) {
    printf("channel %d : %s\n", pos->channel_id, pos->description);
  }

  free(msg_list); // 释放接收的节目单包

  // 用户选择频道不需要上传到服务端
  int chosen_channel_id; // 用户选择频道
  while (1) {
    int ret = scanf("%d", &chosen_channel_id);
    if (ret != 1)
      exit(EXIT_FAILURE);
  }

  // 6-3 收频道包, 发送给子进程
  struct msg_channel_st *msg_channel;
  msg_channel = malloc(MSG_CHANNEL_MAX);
  if (!msg_channel) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in remote_address;
  socklen_t remote_address_len = sizeof(remote_address);
  size_t rmt_len;
  while (1) {
    rmt_len =
        recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void *)&remote_address, &remote_address_len);
    // 若接收数据包的地址或端口不是 server 地址和端口
    if (remote_address.sin_addr.s_addr != server_address.sin_addr.s_addr
        || remote_address.sin_port != server_address.sin_port) {
      fprintf(stderr, "Ignore: address not match.\n");
      continue; // 再次尝试
    }
    // 接收的数据包太小
    if (rmt_len < sizeof(struct msg_channel_st)) {
      fprintf(stderr, "Ignore: message too small.\n");
      continue;
    }
    // 接收的数据包内的频道 id 是否为我选择的频道 id, 若是就写管道, 若不是就从循环头再次接收
    if (msg_channel->channel_id == chosen_channel_id) {
      fprintf(stdout, "accepted msg: %d received.\n", msg_channel->channel_id);   // 测试用语句
      if (writen(pd[1], msg_channel->date, rmt_len - sizeof(channel_id_t)) < 0) { // 写入管道
        exit(EXIT_FAILURE);
      }
    }
  }

  free(msg_channel);
  close(sd);

  exit(EXIT_SUCCESS);
}
