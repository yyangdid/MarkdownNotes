# 进程间共享内存的实现

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 使用内存映射实现父子进程间的共享内存.
  - 运行: `./main`.

- 知识点:
  - `mmap(2)`
  - `munmap(2)`

- 资料:
  - _(LHQ 32.12)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 运行: `./main`.

- 知识点:
  - XSI IPC `Shared Memory Segments`: 共享内存段.
  - 具有亲缘关系的进程间通信.

- 资料:
  - _(LHQ 34.7)_
