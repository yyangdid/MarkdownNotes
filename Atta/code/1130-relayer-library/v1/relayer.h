#include <stdlib.h>

#ifndef RELAYER___H___
  #define RELAYER___H___

// 任务数组大小, 即: 最多可以有多少个任务
  #define RELAYER_JOB_MAX (1000)

// 任务状态
enum {
  STATE_RUNNING = 1, // 正常运行态
  STATE_CANCELED,    // 取消态
  STATE_OVER         // 结束态
};

// 任务状态结构体, 给用户看的
struct relayer_stat_st {
  int state;                // 状态
  int fd1;                  // 任务中的文件描述符 1
  int fd2;                  // 任务中的文件描述符 2
  int64_t count12, count21; // 1 对 2 传输的字符个数, 2 对 1 传输的字符个数
  // struct timeval start, end; // 任务开始时间和结束时间(可选)
};

/**
 * @brief 向任务数组中添加任务.
 * @param fd1[in] 文件描述符 1
 * @param fd2[in] 文件描述符 2
 * @return 数组的下标.
 * - >= 0 : 成功, 返回当前任务的 ID.
 * - == -EINVAL : 失败, 参数非法.
 * - == -ENOSPC : 失败, 任务数组已满.
 * - == -ENOMEM : 失败, 内存分配失败.
 */
int relayer__addJob(int fd1, int fd2);

/**
 * @brief 取消任务.
 * @param id[in] 要取消任务的 ID.
 * @return 是否成功.
 * - == 0 : 成功, 指定任务成功取消.
 * - == -EINVAL : 失败, 参数非法.
 * - == -EBUSY : 失败, 任务早已被取消.
 */
int relayer__cancelJob(int id);

/**
 * @brief 为指定任务清理资源.
 * @param id[in] 要清理资源的任务 ID.
 * @param relay[in, out] 用于存放任务状态的任务结构体地址.
 * @return 执行结果.
 * - == 0 : 成功, 指定任务已终止并返回状态.
 * - == -EINVAL : 失败, 参数非法.
 */
int relayer__waitJob(int id, struct relayer_stat_st *relay);

/**
 * @brief 获取任务当前的状态.
 * @param id[in] 要获取状态的任务 ID.
 * @param relay[in, out] 用于存放任务状态的任务结构体地址.
 * @return 执行结果.
 * - == 0 : 成功, 指定任务状态已经返回.
 * - == -EINVAL : 失败, 参数非法.
 */
int relayer__statJob(int id, struct relayer_stat_st *relay);

#endif // RELAYER___H___
