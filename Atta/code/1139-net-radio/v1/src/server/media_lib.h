#ifndef MEDIA_LIB___H___
#define MEDIA_LIB___H___

#include "set_type.h"

// -------------------------------------------------------------------------------------------------
// 各种结构体定义
// 媒体库频道条目结构体
struct media_lib_listEntry_st {
  channel_id_t channel_id; // 频道 id
  char *description;       // 频道描述
};

// -------------------------------------------------------------------------------------------------
// 各函数声明
/**
 * @brief 获取媒体库频道列表.
 * @param list[in,out] 媒体库频道列表指针的地址.
 * @param list_num[in,out] 频道数量.
 * @return 执行状态: 0: 成功.
 */
int media_lib__getChannelList(struct media_lib_listEntry_st **list, int *list_num);

/**
 * @brief 销毁媒体库频道列表.
 * @param list[in,out] 要销毁媒体库频道列表的地址.
 * @return 执行状态:
 */
int media_lib__freeChannelList(struct media_lib_listEntry_st *list);

/**
 * @brief 从 channel_id 中读取频道信息写入 str 中, 读取 size 字节.
 * @param channel_id[in] 频道 id.
 * @param str[in,out] 存放读取信息的地址.
 * @param size[in] 读取多少字节.
 * @return 成功读取的长度.
 */
ssize_t media_lib__readChannel(channel_id_t channel_id, void *str, size_t size);

#endif // MEDIA_LIB___H___
