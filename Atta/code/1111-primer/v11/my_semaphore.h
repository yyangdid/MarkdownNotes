#ifndef MY_SEMAPHORE___H___
#define MY_SEMAPHORE___H___

typedef void my_sem_t;

/**
 * @brief 初始化信号量.
 * @param init_value[in] 资源总量值.
 * @return
 */
my_sem_t *my_sem__init(int init_value);

/**
 * @brief 归还 num 个资源量到 sem 中.
 * @param sem[in,out] 信号量.
 * @param num[in] 要归还的资源量值.
 * @return 归还资源量的个数
 */
int my_sem__add(my_sem_t *sem, int num);

/**
 * @brief 从 sem 中取 num 个资源量.
 * @param sem[in,out] 信号量.
 * @param num[in] 希望获取的资源量值.
 * @return 获取资源量的个数
 */
int my_sem__sub(my_sem_t *sem, int num);

/**
 * @brief 销毁信号量.
 * @param sem[in,out] 要销毁的信号量.
 * @return 销毁状态: 0 为成功
 */
int my_sem__destroy(my_sem_t *sem);

#endif // MY_SEMAPHORE___H___
