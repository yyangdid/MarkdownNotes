# 令牌桶流量控制库

## v1

- 代码文件：
  - [my_tbf.h](./v1/my_tbf.h)
  - [my_tbf.c](./v1/my_tbf.c)
  - [main.c](./v1/main.c)

- 说明：
  - 令牌桶流量控制库(my_tbf)
  - 运行: `./main /etc/services`

- 知识点:

- 资料:
  - _(LHQ 30.9 30.10 30.11)_

## v2

- 代码文件：
  - [my_tbf.h](./v2/my_tbf.h)
  - [my_tbf.c](./v2/my_tbf.c)
  - [main.c](./v2/main.c)

- 说明：
  - 修改 [v1](#v1) 为多线程版本, 去除所有信号的内容, 用线程机制替换信号机制.
  - 令牌桶流量控制库(my_tbf)
  - 运行: `./main /etc/services`
  - 该程序是盲等版, 非盲等版见 [v3](#v3)

- 知识点:
  - 互斥量.

- 资料:
  - _(LHQ 31.10 31.11)_

## v3

- 代码文件：
  - [my_tbf.h](./v3/my_tbf.h)
  - [my_tbf.c](./v3/my_tbf.c)
  - [main.c](./v3/main.c)

- 说明：
  - 修改 [v2](#v2) 为非盲等版.
  - 令牌桶流量控制库(my_tbf)
  - 运行: `./main /etc/services`

- 知识点:
  - 互斥量和条件变量.

- 资料:
  - _(LHQ 31.12)_
