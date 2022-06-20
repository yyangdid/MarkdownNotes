#ifndef TBF___H___
#define TBF___H___

// -------------------------------------------------------------------------------------------------
// 宏定义
// 令牌桶个数上限, 即: 令牌桶数组大小
#define TBF_MAX (1024)

// -------------------------------------------------------------------------------------------------
// 别名定义
// 令牌桶类型
typedef void tbf_t;

// -------------------------------------------------------------------------------------------------
// 函数声明

/**
 * @brief 初始化令牌桶
 * @param cps[in] 传输速率.
 * @param burst[in] 令牌上限.
 * @return 成功初始化令牌桶的地址, 初始化失败返回 NULL.
 */
tbf_t *tbf__init(int cps, int burst);

/**
 * @brief 从令牌桶 tbf 中获取 num 个令牌.
 * @param tbf[in,out] 令牌桶地址.
 * @param num[in] 令牌数量.
 * @return 成功获取的令牌个数.
 */
int tbf__fetchToken(tbf_t *tbf, int num);

/**
 * @brief 将 num 个令牌归还到令牌桶 tbf.
 * @param tbf[in,out] 令牌桶地址.
 * @param num[in] 令牌数量.
 * @return 执行状态, 0: 成功.
 */
int tbf__returnToken(tbf_t *tbf, int num);

/**
 * @brief 销毁令牌桶 tbf.
 * @param tbf[in,out] 要销毁的令牌桶.
 * @return 执行状态, 0: 成功.
 */
int tbf__destroy(tbf_t *tbf);

#endif // TBF___H___
