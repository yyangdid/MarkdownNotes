#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "proto.h"

int main() {
  key_t key = ftok(KEY_PATH, KEY_PROJ); // 获得同一个 key
  if (key < 0) {                        // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  int msg_id = msgget(key, IPC_CREAT | 0600); // 创建消息队列实例, 获得消息队列标识符
  if (msg_id < 0) {                           // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  struct msg_st rcv_buff;
  while (1) {
    if (msgrcv(msg_id, &rcv_buff, sizeof(rcv_buff) - sizeof(long), 0, 0) < 0) { // 接收消息
      fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
    printf("Name: %s\n", rcv_buff.name);
    printf("Math: %d\n", rcv_buff.math);
    printf("Chinese: %d\n", rcv_buff.chinese);
  }

  // 销毁消息队列实例, 需校验返回值, 这里省略了
  // 永远无法运行到这里, 程序只能强制退出异常终止, 使用 `ipcs` 查看发现消息队列居然还在, 需使用
  // `ipcrm` 命令(`ipcrm -q XXmsqidXX`)进行删除
  // 需要使用信号处理正常终止(守护进程里有示例)
  msgctl(msg_id, IPC_RMID, NULL);


  exit(EXIT_SUCCESS);
}
