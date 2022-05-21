# 计算一个文件的大小

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 使用 `fopen(3) fgetc(3) fclose(3)` 实现计算一个文件的大小。
  - 运行：`./main ~/.bashrc`

- 知识点一：
  - 程序运行的命令行参数检查
  - `strcmp(3)`
  - `fopen(3)`
  - `fclose(3)`
  - `fgetc(3)`

- 资料：
  - _(LHQ 25.5)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 使用 `fseek(3) ftell(3)` 代替 `fopen(3) fgetc(3) fclose(3)`，一步到位，效率提升无数倍。
  - 运行：`./main ~/.bashrc`
  - 只能用于小于 2G 的文件。
    - 可以使用 `fseeko(3)` 和 `ftello(3)` 代替 `fseek(3) ftell(3)`。（具体要看系统实现，看 man 手册）。
      但是这两个函数不属于 C 标准，所以需要考虑可移植性问题。

- 知识点一：
  - `fseek(3)`
  - `ftell(3)`

- 知识点二： _(LHQ 25.8)_
  - `fseeko(3)`
  - `ftello(3)`

- 资料：
  - _(LHQ 25.9)_

## v3

- 代码文件：
  - [main.c](./v3/main.c)

- 说明：
  - 使用 `stat(2)` 可以直接获取文件的大小.
  - 运行：`./main ~/.bashrc`

- 知识点一：
  - `stat(2)`

- 资料：
  - _(LHQ 27.3)_
