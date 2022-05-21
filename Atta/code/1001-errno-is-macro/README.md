# 验证 errno 现在已经被私有化重构为宏了

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 该代码不可能被编译和运行。
  - 使用 `gcc -E main.c` 进行预处理。
  - 如果在预处理后的代码中还能够看到 `errno` 这个变量的话就说明它还是一个整型的变量，实际上在预处理
    过的代码中发现 `errno` 已经被替换为 `(*__errno_location ())` 了，是一个私有化数据了。
  - 关于 `私有化数据` 可以参考 `APUE 12.6 线程特定数据` 和 `TLPI 31.3 线程特有数据`。

- 资料：
  - _(LHQ 25.2)_