#ifndef PROTO___H___
#define PROTO___H___

#include "set_type.h"

// -------------------------------------------------------------------------------------------------
// 各种宏定义
// ----------------------------------------------------------------------------
// 多播组默认地址
#define DEFAULT_MUT_GROUP    "224.2.2.2"
// 多播组默认端口
#define DEFAULT_RCV_PORT     "1989"
// 总频道个数
#define CHANNEL_TOTAL_NUM    100
// 0 号频道往外发送节目单
#define CHANNEL_LIST_ID      0
// 最小频道 id 号
#define CHANNEL_MIN_ID       1
// 最大频道 id 号
#define CHANNEL_MAX_ID       (CHANNEL_MIN_ID + CHANNEL_TOTAL_NUM - 1)

// ----------------------------------------------------------------------------
// msg_channel_st 结构体的最大大小
// UDP 推荐长度为 512~64K
// 减 20 为减去 IP 包的报头. 20 为 IP 包报头的推荐长度
// 减 8 为减去 UDP 包的报头
#define MSG_CHANNEL_MAX      (65536 - 20 - 8)
// msg_channel_st 中 data 数据包的最大大小
#define MSG_CHANNEL_DATA_MAX (MSG_CHANNEL_MAX - sizeof(channel_id_t))

// ----------------------------------------------------------------------------
// msg_channel_st 结构体的最大大小
// UDP 推荐长度为 512~64K
// msg_list_st 结构体的最大大小
#define MSG_LIST_MAX         (65536 - 20 - 8)
// msg_list_st 中 list_entry 数据包的最大大小
#define MSG_LIST_ENTRY_MAX   (MSG_LIST_MAX - sizeof(channel_id_t))

// -------------------------------------------------------------------------------------------------
// 各种结构体定义
// ----------------------------------------------------------------------------
// msg_channel_st 结构体的最大大小
// UDP 推荐长度为 512~64K
// 往外发送数据包的结构体
struct msg_channel_st {
  channel_id_t channel_id; // 发送数据的频道 id 号, 值为 [CHANNEL_MIN_ID, CHANNEL_MAX_ID]
  uint8_t date[1]; // 传输的数据, 变长数组. c99 之前不支持数组的长度为 0, 为了兼容性这里写为了 1
} __attribute__((packed)); // 告诉 Gcc 该结构体不需要对齐

// ----------------------------------------------------------------------------
// msg_channel_st 结构体的最大大小
// UDP 推荐长度为 512~64K
// 每一个频道 id 号的信息
struct msg_list_entry_st {
  channel_id_t channel_id; // 频道 id 号.
  uint16_t len;            // 当前包的长度
  uint8_t description[1];  // 频道的描述, 变长数组
} __attribute__((packed));

// ----------------------------------------------------------------------------
// msg_channel_st 结构体的最大大小
// UDP 推荐长度为 512~64K
// 节目单结构体
struct msg_list_st {
  channel_id_t channel_id;                // 发送节目单的 id 号, 一定为 CHANNEL_LIST_ID
  struct msg_list_entry_st list_entry[1]; // 若干个频道的信息, 变长结构体数组

} __attribute__((packed));

#endif // PROTO___H___
