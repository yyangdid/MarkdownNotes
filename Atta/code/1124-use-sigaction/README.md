# sigaction(2) 函数的使用

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 修改自 [守护进程 v2](../1116-my-daemon/README.md#v2), 加入信号处理.
  - `signal(2)` 函数有重入的危险, [v2](#v2) 中使用 `sigaction(2)` 来代替.

- 知识点
  - `signal(2)`
  - `sigaction(2)`

- 资料:
  - _(LHQ 30.18)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 修改自 [v1](#v1)
  - 使用 `sigaction(2)` 来代替 `signal(2)`.

- 知识点
  - `signal(2)`
  - `sigaction(2)`
  - `setitimer(2)`

- 资料:
  - _(LHQ 30.18)_

## v3

- 代码文件：
  -[my_tbf.h](./v3/my_tbf.h)
  -[my_tbf.c](./v3/my_tbf.c)
  - [main.c](./v3/main.c)

- 说明：
  - [令牌桶流量控制库 v1](../1120-my-token-bucket-flow-control-library/README.md#v1) 缺陷见 _(LHQ 30.18(17:36))_
  - 使用 `sigaction(2)` 进行改写(只需修改[my_tbf.c](./v3/my_tbf.c)).

- 知识点
  - `sigaction(2)`
  - `setitimer(2)`

- 资料:
  - _(LHQ 30.18 30.19)_
