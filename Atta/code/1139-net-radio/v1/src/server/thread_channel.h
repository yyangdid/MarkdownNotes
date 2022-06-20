#ifndef THREAD_CHANNEL___H___
#define THREAD_CHANNEL___H___

#include "media_lib.h"

// -------------------------------------------------------------------------------------------------
// 各函数定义
// 创建频道线程
// 媒体库频道列表指针
/**
 * @brief 创建频道线程.
 * @param list[in] 要创建的频道指针.
 * @return 执行状态:
 */
int thread_channel__create(struct media_lib_listEntry_st *list);

/**
 * @brief 销毁频道线程.
 * @param list[in,out] 要销毁的频道指针.
 * @return 执行状态:
 */
int thread_channel__destroy(struct media_lib_listEntry_st *list);

/**
 * @brief 销毁所有频道.
 * @return 执行状态:
 */
int thread_channel__destroyAll(void);

#endif // THREAD_CHANNEL___H___
