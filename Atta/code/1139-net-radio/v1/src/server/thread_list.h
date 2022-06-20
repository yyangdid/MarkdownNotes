#ifndef THREAD_LIST___H___
#define THREAD_LIST___H___

#include "media_lib.h"

// -------------------------------------------------------------------------------------------------
// 各函数声明
/**
 * @brief 创建节目单线程.
 * @param list[in] 媒体库频道列表指针.
 * @param list_size [in] 频道数量.
 * @return 执行状态:
 */
int thread_list__create(struct media_lib_listEntry_st *list, int list_size);

/**
 * @brief 销毁节目单线程.
 * @return 执行状态:
 */
int thread_list__destroy(void);

#endif // THREAD_LIST___H___
