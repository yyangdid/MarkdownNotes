/**
 * @file server.c
 * @brief 被动端, 先收后发.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "proto.h"

#define IP_STR_SIZE (40)
#define BUFF_SIZE   (1024)

// 向已连接的客户端发送信息
static void server_job(int sd) {
  char buff[BUFF_SIZE];
  int len = sprintf(buff, FMT_STAMP, (long long)time(NULL));
  if (send(sd, buff, len, 0) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  // 1. 获取 SOCKET
  int sd = socket(AF_INET, SOCK_STREAM, 0 /* 0 代表IPPROTO_TCP, 也可以用后来退出的 IPPROTO_SCTP */);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  // 设置 SOCKET 属性
  // 若不设置 SO_REUSEADDR 属性, 当运行 server 后使用 `nc 127.0.0.1 1989` 命令测试连接, 使用 Ctrl+c
  // 强制关闭 server, 此时马上再次运行 server 会报错端口忙, 使用 `netstat -ant` 查看已使用 TCP 端口
  // 状态, server 强制退出后, 其端口正在等待系统回收. 等一会再运行 server 就不会报错了, 这个报错是
  // 绑定时的保存.
  // 设置 SO_REUSEADDR 属性的作用是,若 bind(2) 是发现要绑定的 ip:port 处于未使用正在等待系统回收的状
  // 态时, 可以进行重新使用(不回收旧的, 依然是等待系统回收, 可以使用 netstat -ant 查看).
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
  // 第二参数原本代表半连接池的大小, 现在代表 server 端 socket 最多可以监听多少个客户端全连接
  if (listen(sd, 200) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 4. 接受连接
  struct sockaddr_in remote_address;
  socklen_t remote_address_len = sizeof(remote_address);
  while (1) {
    // 第二个参数为客户端的地址
    // int new_sd 为一个文件描述符
    int new_sd = accept(sd, (void *)&remote_address, &remote_address_len);
    if (new_sd < 0) {
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }

    char ip_str[IP_STR_SIZE];
    inet_ntop(AF_INET, &remote_address.sin_addr, ip_str, IP_STR_SIZE);
    printf("Client: %s:%d\n", ip_str, ntohs(remote_address.sin_port));

    // 5. 收/发消息
    // 把 send(2) 放到了 server_job() 函数中了
    server_job(new_sd); // 向客户端发送信息

    close(new_sd); // 关闭文件描述符
  }

  // 6. 关闭 SOCKET
  close(sd);

  exit(EXIT_SUCCESS);
}
