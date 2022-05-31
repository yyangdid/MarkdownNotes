# 线程池类写法

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 20 个线程并发从同一个文件中读取数据, 对数据加一, 然后覆盖写入文件.
  - 运行: `./main`. 程序运行前需存在文件 `/home/yangdi/zz` 且文件内容为一个数字.
  - 该程序有线程竞争, 不能得到正确结果.

- 知识点
  - 线程竞争.

- 资料:
  - _(LHQ 31.7)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 对 [v1](#v1) 进行改写, 加入互斥量的使用.
  - 运行: `./main`. 程序运行前需存在文件 `/home/yangdi/zz` 且文件内容为一个数字.

- 知识点
  - 使用互斥量解决线程竞争.
  - `pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;` : 静态初始化互斥量.
  - `pthread_mutex_init(3p)` : 动态初始化互斥量.
  - `pthread_mutex_destroy(3p)`
  - `pthread_mutex_lock(3p)` : 阻塞.
  - `pthread_mutex_trylock(3p)` : 非阻塞.
  - `pthread_mutex_unlock(3p)`
    - `pthread_mutex_lock(3p)` 和 `pthread_mutex_unlock(3p)` 之间的内容一般称为临界区.

- 资料:
  - _(LHQ 31.8)_

## v3

- 代码文件：
  - [main.c](./v3/main.c)

- 说明：
  - 对 [v2](#v2) 进行改写, 将多线程并发改为多进程并发.
  - 运行: `./main`. 程序运行前需存在文件 `/home/yangdi/zz` 且文件内容为一个数字.

- 知识点
  - 文件锁

- 资料:
  - _(LHQ 32.13)_
