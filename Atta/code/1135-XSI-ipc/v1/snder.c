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

  int msg_id = msgget(key, 0); // 获取消息队列标识符, 不创建
  if (msg_id < 0) {            // 失败
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  struct msg_st snd_buff;
  snd_buff.m_type = 1;
  strcpy(snd_buff.name, "Alan");
  snd_buff.math    = rand() % 100;
  snd_buff.chinese = rand() % 100;
  if (msgsnd(msg_id, &snd_buff, sizeof(snd_buff) - sizeof(long), 0) < 0) { // 发送消息
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // msgctl(); // 销毁消息队列实例. 因为没有创建, 所以不需要销毁
  printf("Ok!\n");

  exit(EXIT_SUCCESS);
}
