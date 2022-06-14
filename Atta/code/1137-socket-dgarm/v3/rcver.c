/**
 * @file rcver.c
 * @brief 接收方, 被动端.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "proto.h"
#define IP_STR_SIZE 40

int main(void) {
  // 取得 SOCKET
  // 使用 AF_INET 协议族中默认支持 SOCK_DGRAM 的协议来实现
  int sd; // 文件描述符
  sd = socket(AF_INET, SOCK_DGRAM, 0 /* 0 相当于 IPPROTO_UDP*/);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 设置 socket 属性,
  int val = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 给 SOCKET 绑定地址
  struct sockaddr_in local_addr; // 我端地址
  local_addr.sin_family = AF_INET;
  local_addr.sin_port   = htons(strtol(RCV_PORT, NULL, 10));
  inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr);
  if (bind(sd, (void *)&local_addr, sizeof(local_addr)) < 0) {
    close(sd);
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 接收
  struct msg_st recv_buff;
  struct sockaddr_in remote_addr;                  // 对端地址
  socklen_t remote_addr_len = sizeof(remote_addr); // 一定要初始化, 否则第一次获取第一是错误的
  char ip_str[IP_STR_SIZE];
  while (1) {
    recvfrom(sd, &recv_buff, sizeof(recv_buff), 0, (void *)&remote_addr, &remote_addr_len);
    inet_ntop(AF_INET, &remote_addr.sin_addr, ip_str, IP_STR_SIZE);
    printf("--- MESSAGE FROM %s:%d ---\n", ip_str, ntohs(remote_addr.sin_port));
    printf("Name    = %s\n", recv_buff.name); // 单字节传输不区分大小端, 无需本地与网络端转换
    printf("Math    = %d\n", ntohl(recv_buff.math));
    printf("Chinese = %d\n", ntohl(recv_buff.chinese));
  }

  // 关闭 SOCKET
  close(sd);

  exit(EXIT_SUCCESS);
}
