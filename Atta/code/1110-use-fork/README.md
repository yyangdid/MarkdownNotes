# fork(2) 的使用

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 运行: `./main`
  - 运行: `./main > ~/a.txt` 对输出进行重定向

- 知识点:
  - `getpid(2)`
  - `ps axf`
  - **在 `fork(2)` 之前一定要记得使用 `fflush(NULL)` 刷新所有成功打开的流**

- 资料:
  - _(LHQ 29.2)_
