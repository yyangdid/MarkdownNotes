#ifndef PROTO___H___
#define PROTO___H___

#define KEY_PATH  "/etc/services"
#define KEY_PROJ  'g'
#define NAME_SIZE 32

struct msg_st {
  long m_type; // 当前消息的类型, 在当前程序中没有什么作用
  char name[NAME_SIZE];
  int math;
  int chinese;
};

#endif // PROTO___H___
