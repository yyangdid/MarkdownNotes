#ifndef PROTO___H___
#define PROTO___H___

#include <stdint.h>

#define NAME_SIZE (11)
// 接收方端口
#define RCV_PORT  "1989"
// 多播组的组号
#define MUT_GROUP "224.2.2.2"

struct msg_st {
  // char name[NAME_SIZE];
  // int math;
  // int chinese;
  uint8_t name[NAME_SIZE];
  uint32_t math;
  uint32_t chinese;
} __attribute__((packed)); // 告诉 Gcc 不对结构体进行字节序对齐

#endif // PROTO___H___
