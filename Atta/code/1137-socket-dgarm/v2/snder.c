/**
 * @file snder.c
 * @brief 发送方, 主动端.
 * @version 0.0.2
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
  if (argc < 3 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s ip_address name\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 判断命令行第三个参数的长度是否大于结构体 name 成员允许的最大长度
  if (strlen(argv[2]) > NAME_MAX) {
    fprintf(stderr, "Name is too long!\n");
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
  // struct msg_st send_buf;
  struct msg_st *send_buf_p;
  size_t size = sizeof(struct msg_st) + strlen(argv[2]);
  send_buf_p  = malloc(size);
  if (send_buf_p == NULL) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  strcpy((char *)send_buf_p->name, argv[2]);
  send_buf_p->math    = htonl(rand() % 100);
  send_buf_p->chinese = htonl(rand() % 100);
  struct sockaddr_in remote_addr; // 远端地址
  remote_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &remote_addr.sin_addr);
  remote_addr.sin_port = htons(strtol(RCV_PORT, NULL, 10));
  if (sendto(sd, send_buf_p, size, 0, (void *)&remote_addr, sizeof(remote_addr)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  printf("OK!\n");

  // 关闭 SOCKET
  close(sd);

  exit(EXIT_SUCCESS);
}
