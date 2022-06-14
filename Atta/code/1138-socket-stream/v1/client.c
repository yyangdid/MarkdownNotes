/**
 * @file client.c
 * @brief 主动端, 先发后收.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "proto.h"

int main(int argc, char *argv[]) {
  // 检查命令行参数
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s address\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 1. 获取 SOCKET
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 2. 给 SOCKET 取得地址(可省略)

  // 3. 发起连接
  struct sockaddr_in remote_address;
  remote_address.sin_family = AF_INET;
  remote_address.sin_port   = htons(strtol(SERVER_PORT, NULL, 10));
  inet_pton(AF_INET, argv[1], &remote_address.sin_addr);
  if (connect(sd, (void *)&remote_address, sizeof(remote_address)) < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 4. 发/收消息
  // recv(); // 可以这么写, 本例没有使用, 而是使用了另外的思路

  // 5. 关闭 SOCKET
  // close(); // 可以这么写, 本例没有使用, 而是使用了另外的思路

  // 将 SOCKET 文件描述符转换为 文件流, 接下来所有操作都可使用标准 I/O 了
  // `一切皆文件` 这就用上了 :)
  FILE *fp = fdopen(sd, "r+");
  if (!fp) {
    // 挂钩子函数, 或关闭各种资源, 这里略过
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  long long stamp;
  if (fscanf(fp, FMT_STAMP, &stamp) < 1) {
    fprintf(stderr, "%s->%d Errors\n", __FILE__, __LINE__);
    fflush(NULL);
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "stamp = %lld\n", stamp);
  }

  fclose(fp);

  exit(EXIT_SUCCESS);
}
