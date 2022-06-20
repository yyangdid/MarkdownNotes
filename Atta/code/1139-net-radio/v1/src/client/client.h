#ifndef CLIENT___H___
#define CLIENT___H___

// -------------------------------------------------------------------------------------------------
// client 端用到的各种宏定义
// 默认的命令行播放器, 把 mpg123 默认的输出重定向到黑洞
// 解码器的默认运行路径
#define DEFAULT_PLAYER_CMD "/usr/bin/mpg123 -  > /dev/null"

// -------------------------------------------------------------------------------------------------
// 全局变量
// 对 client.c 中定义的全局变量进行声明, 让其他 .c 文件可以直接使用 client_conf 实例
extern struct client_conf_st client_conf;

// -------------------------------------------------------------------------------------------------
// 各种结构体定义
// client 端默认的命令行参数结构体
struct client_conf_st {
  char *rcv_port;   // 接收端口
  char *mut_group;  // 多播地址
  char *player_cmd; // 播放器命令行
};

#endif // CLIENT___H___
