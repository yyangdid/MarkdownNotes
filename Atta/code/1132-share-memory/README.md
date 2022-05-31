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