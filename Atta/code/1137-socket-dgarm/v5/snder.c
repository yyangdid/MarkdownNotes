/**
 * @file snder.c
 * @brief 发送方, 主动端.
 * @version 0.0.5
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

int main(int argc, char *argv[]) {
  // 检查命令行参数
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s ip_addr\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 取得 SOCKET
  int sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 给 SOCKET 绑定地址, 发送方可以省略
  // bind();

  // 发送
  struct msg_st send_buf;
  memset(&send_buf, '\0', sizeof(send_buf)); // 清空 send_buf, 防止发送多余数据, 造成数据外泄
  strcpy((char *)send_buf.name, "Alan");
  send_buf.math    = htonl(rand() % 100);
  send_buf.chinese = htonl(rand() % 100);
  struct sockaddr_in remote_addr; // 远端地址
  remote_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &remote_addr.sin_addr);
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
