# 简单的 cat 程序实现

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 对 [简单的 copy 程序实现 v5](../1020-my-copy/README.md#v5) 进行改写. (未加入信号内容, 在
    [v2](#v2) 中加入了信号的使用)
  - 运行: `./main /etc/services`

- 知识点:

- 资料:
  - _(LHQ 30.7)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 在 [v1](#v1) 中加入了信号的使用, 每秒钟输出固定字符(即: 流量控制, 流控, 漏桶.不要一下把所有的都
    输出).
  - 运行: `./main /etc/services`

- 知识点:
  - 流量控制, 流控
  - 漏桶实现

- 资料:
  - _(LHQ 30.8)_

## v3

- 代码文件：
  - [main.c](./v3/main.c)

- 说明：
  - 在 [v1](#v1) 中加入了信号的使用, 每秒钟输出固定字符(即: 流量控制, 流控, 令牌桶.不要一下把所有的
    都输出).
  - 运行: `./main /etc/services`

- 知识点:
  - 流量控制, 流控
  - 令牌桶实现

- 资料:
  - _(LHQ 30.9)_

## v4

- 代码文件：
  - [main.c](./v4/main.c)

- 说明：
  - 修改[v3](#v3), 修改令牌为信号原子类型.
  - 运行: `./main /etc/services`

- 知识点:
  - 流量控制, 流控
  - 令牌桶实现
  - 原子类型

- 资料:
  - _(LHQ 30.9)_
  - _(LHQ 30.13(15:30))_

## v5

- 代码文件：
  - [main.c](./v5/main.c)

- 说明：
  - 修改[v2](#v2), 修改 `alarm(2)` 为 `setitimer(2)`.
  - 运行: `./main /etc/services`

- 知识点:
  - `setitimer(2)`

- 资料:
  - _(LHQ 30.14)_