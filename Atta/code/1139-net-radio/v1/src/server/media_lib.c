#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "media_lib.h"
#include "proto.h"
#include "server.h"
#include "tbf.h"

#define PATH_SIZE 1024

// -------------------------------------------------------------------------------------------------
// 对外隐藏的结构体
// 频道信息
struct channel_context_st {
  channel_id_t channel_id;
  char *description;
  glob_t mp3_glob; // 解析媒体库目录
  int pos;         // 当前播放的音乐
  int fd;          // 当前播放音乐的文件描述符
  off_t offset;    // 数据发送到哪个位置
  tbf_t *tbf;      // 流量控制
};

static struct channel_context_st channel[CHANNEL_MAX_ID + 1];

// -------------------------------------------------------------------------------------------------
// 函数实现
/*
 * @brief 获取媒体库频道列表.
 * @param list[in,out] 媒体库频道列表指针的地址.
 * @param list_num[in,out] 频道数量.
 * @return 执行状态: 0: 成功.
 */
int media_lib__getChannelList(struct media_lib_listEntry_st **list, int *list_num) {
  for (int i = 0; i < CHANNEL_MAX_ID + 1; i++) {
    channel[i].channel_id = -1; // -1 表示当前频道未启用
  }
  char path[PATH_SIZE];
  snprintf(path, PATH_SIZE, "%s/*", server_conf.media_dir); // 测试语句
  glob_t glob_rst;
  if (glob(path, 0, NULL, &glob_rst))
    return -1;

  struct media_lib_listEntry_st *entry;
  struct channel_context_st *context;
  entry = malloc(sizeof(struct media_lib_listEntry_st) * glob_rst.gl_pathc);
  if (entry == NULL) {
    syslog(LOG_ERR, "%s->%d Error: malloc().", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  int num = 0;
  for (int i = 0; i < glob_rst.gl_pathc; i++) {
    // 每个 glob_rst.gl_pathv[i] 对应一个 "/home/yangdi/ch1" 目录名
    path2entry(glob_rst.gl_pathv[i]);
    num++;
  }

  *list = ;

  *list_num = num;

  return 0;
}

/*
 * @brief 销毁媒体库频道列表.
 * @param list[in,out] 要销毁媒体库频道列表的地址.
 * @return 执行状态:
 */
int media_lib__freeChannelList(struct media_lib_listEntry_st *list) {}

/*
 * @brief 从 channel_id 中读取频道信息写入 str 中, 读取 size 字节.
 * @param channel_id[in] 频道 id.
 * @param str[in,out] 存放读取信息的地址.
 * @param size[in] 读取多少字节.
 * @return 成功读取的长度.
 */
ssize_t media_lib__readChannel(channel_id_t channel_id, void *str, size_t size) {}
