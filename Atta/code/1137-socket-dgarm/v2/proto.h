#ifndef PROTO___H___
#define PROTO___H___

#include <stdint.h>

// #define NAME_SIZE (11)
// 包的固有长度减去结构体固有成员的长度
// 512 是 UDP 包的推荐长度, 减去的第一个 8 是 math 和 chinese 成员的长度, 第二个 8 是 UDP 包的报头
#define NAME_MAX (512 - 8 - 8)
// 接收方端口
#define RCV_PORT "1989"

struct msg_st {
  // char name[NAME_SIZE];
  // int math;
  // int chinese;
  // uint8_t name[NAME_SIZE];
  uint32_t math;
  uint32_t chinese;
  uint8_t name[1];         // 变长结构体, 这里的 name 就是一个占位符
} __attribute__((packed)); // 告诉 Gcc 不对结构体进行字节序对齐

#endif // PROTO___H___
