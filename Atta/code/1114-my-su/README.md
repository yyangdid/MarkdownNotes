# 简易 su 命令

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 首先切换到 root 用户把 `main` 程序的属主设置为 root: `chown root main`.
  - 给 `main` 程序赋予 `u+s` 权限: `chmod u+s main`
  - 回普通用户运行: `./main 0 cat /etc/shadow` 以 root 用户查看文件, 如果没有上面两步会提示没有权限

- 知识点:

- 资料:
  - _(LHQ 29.11)_
