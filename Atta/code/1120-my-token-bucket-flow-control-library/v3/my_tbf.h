#ifndef MY_TBF__H__
#define MY_TBF__H__

// 令牌桶个数上限
#define MY_TBF_MAX (1024)

typedef void my_tbf__t;

/**
 * @brief 初始化令牌桶
 * @param cps[in] 每秒钟传输的字符个数
 * @param burst[in] 令牌个数上限
 * @return 令牌桶结构示例的首地址
 */
my_tbf__t *my_tbf__init(int cps, int burst);

/**
 * @brief 从令牌桶 my_tbf 中取 num 个令牌
 * @param my_tbf[in,out] 令牌桶首地址
 * @param num[in] 令牌个数
 * @return 成功取到的令牌个数
 */
int my_tbf__fetchToken(my_tbf__t *my_tbf, int num);

/**
 * @brief 向令牌桶 my_tbf 归还 num 个令牌
 * @param my_tbf[in,out] 令牌桶首地址
 * @param num[in] 令牌个数
 * @return 成功归还的令牌个数
 */
int my_tbf__returnToken(my_tbf__t *my_tbf, int num);

/**
 * @brief 销毁令牌桶
 * @param my_tbf[in,out] 要销毁令牌桶的首地址
 * @return 是否销毁成功, 0 为成功, 非 0 为失败
 */
int my_tbf__destroy(my_tbf__t *my_tbf);

#endif // MY_TBF__H__
