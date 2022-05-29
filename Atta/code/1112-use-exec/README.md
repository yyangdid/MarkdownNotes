# exec(3) 函数族的使用

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 打印系统时戳(即: date +%s)
  - 运行: `./main`
  - 运行: `./main > ~/aa.txt` 注意带 `fflush(NULL)` 和不带的区别.
  - `pid` 不变.

- 知识点:
  - `exec(3)` 函数族

- 资料:
  - _(LHQ 29.7)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 打印系统时戳(即: date +%s)
  - 运行: `./main`
  - `few` : `fork(2)` `exec(3)` `wait(2)` 综合运用.

- 知识点:

- 资料:
  - _(LHQ 29.7)_

## v3

- 代码文件：
  - [main.c](./v3/main.c)

- 说明：
  - 用 `/usr/bin/sleep` 代替 `/usr/bin/date`
  - 运行: `./main` 修改代码中注释语句, 使用 `ps axf` 观察父子进程关系
  - `few` : `fork(2)` `exec(3)` `wait(2)` 综合运用.

- 知识点:
  - `exec(3)` 中的 `argv[0]` 的名字可以随便取

- 资料:
  - _(LHQ 29.8)_

## v4

- 代码文件：
  - [main.c](./v4/main.c)

- 说明：
  - 基于 [v2](#v2) 修改, 展示 [system(3) 函数的使用 v1](../1115-use-system/README.md#v1) 中的
    `system(3)` 函数换成 `few` 是如何实现的.
  - 打印系统时戳(即: date +%s)
  - 运行: `./main`
  - `few` : `fork(2)` `exec(3)` `wait(2)` 综合运用.

- 知识点:

- 资料:
  - _(LHQ 29.12)_
