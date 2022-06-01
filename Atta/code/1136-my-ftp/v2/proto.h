#ifndef PROTO___H___
#define PROTO___H___

#define KEY_PATH "/etc/services"
#define KEY_PROJ 'a'
#define PATH_MAX (1024)
#define DATA_MAX (1024)

// 包类型
enum { MSG_PATH = 1, MSG_DATA, MSG_EOT };

// 路径包, server 端接收
typedef struct msg_path_st {
  long m_type;         // 包类型, 必须是 MSG_PATH
  char path[PATH_MAX]; // 文件路径, ASCIIZ 带尾零的串
} msg_path_t;

// // 数据包, client 端接收
// typedef struct msg_data_st {
//   long m_type; // 包类型, 必须是 MSG_DATA
//   char data[DATA_MAX];
//   int data_len; // data 中有效数据的长度
// } msg_data_t;
//
// // eot 包, 数据结束包, client 端接收
// typedef struct msg_eot_st {
//   long m_type; // 包类型, 必须是 MSG_EOT
// } msg_eot_t;

typedef struct msg_s2c_st {
  long m_type;  // 包类型, 必须是 MSG_DATA 或 MSG_EOT
  int data_len; // data 中有效数据的长度
  // 若 data_len >0 : data 包
  // 若 data_len == 0 : eot 包
  char data[DATA_MAX];
} msg_s2c_t;

#endif // PROTO___H___
