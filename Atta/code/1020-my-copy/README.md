# 简单的 copy 程序实现

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 运行：`./main ~/.vimrc ~/zz`
  - 使用 `fopen(3) fclose(3) fgetc(3) fputc(3)` 实现一个拷贝文件的程序。

- 知识点：
  - 程序运行的命令行参数检查
  - `strcmp(3)`
  - `fopen(3)`
  - `fclose(3)`
  - `fgetc(3)`
  - `fputc(3)`

- 资料：
  - _(LHQ 25.5)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明
  - 运行：`./main ~/.vimrc ~/zz`
  - 使用 `fgets(3) fputs(3)` 代替 `fgetc(3) fputc(3)`。

- 知识点：
  - `fgets(3)`
  - `fputs(3)`

- 资料：
  - _(LHQ 25.6)_

## v3

- 代码文件：
  - [main.c](./v3/main.c)

- 说明
  - 运行：`./main ~/.vimrc ~/zz`
  - 使用 `fread(3) fwrite(3)` 代替 `fgetc(3) fputc(3)` 或 `fgets(3) fputs(3)`。

- 知识点：
  - `fread(3)`
  - `fwrite(3)`

- 资料：
  - _(LHQ 25.6)_

## v4

- 代码文件：
  - [main.c](./v4/main.c)

- 说明
  - 运行：`./main ~/.vimrc ~/zz`
  - 使用 sysio `open(2) close(2) read(2) write(2)` 进行重写。

- 知识点：
  - `open(2)`
  - `close(2)`
  - `read(2)`
  - `write(2)`

- 资料：
  - _(LHQ 26.3)_

## v5

- 加入信号处理, 对打开, 读取, 写入错误做真假判断.
- [signal(2) 函数的使用 v2](../1117-use-signal/README.md#v2)

## v6

- 加入信号出, 对 [v5](#v5) 进行改写, 实现简单的 cat 程序.
- [简单的 cat 程序实现 v1](../1119-my-cat/README.md#v1)
