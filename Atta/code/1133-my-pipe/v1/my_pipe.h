#ifndef MY_PIPE___H___
#define MY_PIPE___H___

#include <stdlib.h>

// 管道大小
#define PIPE_SIZE      (1024)
// 读者位图
#define MY_PIPE_READER 0x00000001UL
// 写者位图
#define MY_PIPE_WRITER 0x00000002UL

typedef void my_pipe_t;

/**
 * @brief 注册身份, 即: 注册对管道的操作是读者还是写者, 用于管道内计数.
 * @param my_pipe[in, out] 要注册身份到管道的地址.
 * @param opmap[in] 要注册的身份, 位图.
 * @return 是否成功. 0 为成功, 非 0 为失败.
 */
int my_pipe__register(my_pipe_t *my_pipe, int opmap);

/**
 * @brief 注销身份, 即: 注销对管道的操作是读者还是写者, 用于管道内计数.
 * @param my_pipe[in, out] 要注销身份到管道的地址.
 * @param opmap[in] 要注销的身份, 位图.
 * @return 是否成功. 0 为成功, 非 0 为失败.
 */
int my_pipe__unregister(my_pipe_t *my_pipe, int opmap);

/**
 * @brief 初始化管道.
 * @return 成功返回管道的首地址, 失败返回 NULL.
 */
my_pipe_t *my_pipe__init(void);

/**
 * @brief 读管道.
 * @param my_pipe[in,out] 要读取的管道.
 * @param buff[in,out] 存放读取到的内容.
 * @param size[in] 读取多少.
 * @return 实际读取到多少.
 * @note 仿照标准 read(2) 写的.
 */
int my_pipe__read(my_pipe_t *my_pipe, void *buff, size_t size);

/**
 * @brief 写管道.
 * @param my_pipe[in,out] 要写入的管道.
 * @param buff[in] 要写入的内容.
 * @param size[in] 要写入多少.
 * @return 实际写入多少.
 * @note 仿照标准 write(2) 写的.
 */
int my_pipe__write(my_pipe_t *my_pipe, const void *buff, size_t size);

/**
 * @brief 销毁管道.
 * @param my_pipe[in,out] 要销毁管道的地址.
 * @return 执行状态.
 */
int my_pipe__destroy(my_pipe_t *my_pipe);

#endif // MY_PIPE___H___
