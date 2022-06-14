/**
 * @file snder.c
 * @brief 发送方, 主动端.
 * @version 0.0.4
 * @author yyangdid\@gmail.com
 */

#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "proto.h"

int main(void) {
  // 取得 SOCKET
  int sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 设置 SOCKET 属性, 创建多播组
  struct ip_mreqn mut_req;
  inet_pton(AF_INET, MUT_GROUP, &mut_req.imr_multiaddr);
  inet_pton(AF_INET, "0.0.0.0", &mut_req.imr_address);
  mut_req.imr_ifindex = if_nametoindex("eth0");
  if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &mut_req, sizeof(mut_req)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 给 SOCKET 绑定地址, 发送方可以省略
  // bind();

  // 发送
  struct msg_st send_buf;
  strcpy((char *)send_buf.name, "Alan");
  send_buf.math    = htonl(rand() % 100);
  send_buf.chinese = htonl(rand() % 100);
  struct sockaddr_in remote_addr; // 远端地址
  remote_addr.sin_family = AF_INET;
  // inet_pton(AF_INET, argv[1], &remote_addr.sin_addr);
  inet_pton(AF_INET, MUT_GROUP, &remote_addr.sin_addr);
  remote_addr.sin_port = htons(strtol(RCV_PORT, NULL, 10));
  if (sendto(sd, &send_buf, sizeof(send_buf), 0, (void *)&remote_addr, sizeof(remote_addr)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  printf("OK!\n");

  // 关闭 SOCKET
  close(sd);

  exit(EXIT_SUCCESS);
}
