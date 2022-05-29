#ifndef ANY_TIMER__H__
#define ANY_TIMER__H__

// 任务数组大小(即: 可以有多少个任务)
#define JOB_MAX (1024)

typedef void any_timer__jobFunc_t(void *);

/**
 * @brief 添加任务.
 * @param sec[in] 任务等待时间(秒).
 * @param job_ptr[in,out] 任务行为.
 * @param arg[in,out] 任务行为参数.
 * @return 任务在任务数组中的下标.
 * >= 0 : 成功, 返回任务 ID.
 * == -EINVAL : 失败, 参数非法.
 * == -ENOSPC : 失败, 任务数组已满.
 * == -ENOMEM : 失败, 内存空间不足.
 */
int any_timer__addJob(int sec,any_timer__jobFunc_t *job_ptr, void *arg);

/**
 * @brief 取消任务.
 * @param id[in] 要取消的任务.
 * @return 取消状态.
 * - == 0 : 成功, 指定的任务成功取消.
 * - == -EINVAL : 失败, 参数非法.
 * - == -EBUSY : 失败, 指定任务已完成.
 * - == -ECANCELED : 失败, 指定任务重复取消.
 */
int any_timer__cancelJob(int id);

/**
 * @brief 回收释放任务.
 * @param id[in] 要回收的任务 ID.
 * @return 回收释放状态.
 * - == 0 : 成功, 指定任务成功回收释放.
 * - == -EINVAL : 失败, 参数非法.
 */
int any_timer__waitJob(int id);

/* 可以自己添加实现.
 * any_timer__pauseJob(); // 暂停任务.
 * any_timer__resumeJob(); // 继续执行已暂停的任务.
 */

#endif // ANY_TIMER__H__
