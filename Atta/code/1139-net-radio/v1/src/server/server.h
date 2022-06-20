#ifndef SERVER___H___
#define SERVER___H___

// -------------------------------------------------------------------------------------------------
// server 端用到的各种宏定义
#define DEFAULT_MEDIA_DIR "/home/yangdi/media/"
#define DEFAULT_IF        "eth0"
// 运行状态
enum {
  RUN_DAEMON = 1, // 后台运行
  RUN_FOREGROUND  // 前台运行
};

// -------------------------------------------------------------------------------------------------
// 全局变量
// 对 server.c 中定义的全局变量进行声明, 让其他 .c 文件可以直接使用 server_conf 实例
extern struct server_conf_st server_conf;

// -------------------------------------------------------------------------------------------------
// 各种结构体定义
// server 端的结构体
struct server_conf_st {
  char *rcv_port;  // 接收端口
  char *mut_group; // 多播地址
  char *media_dir; // 媒体库位置
  char run_mode;   // 运行方式, 前台或后台. 位图
  char *if_name;   // 从哪个网卡发送数据出去
};

#endif // SERVER___H___
