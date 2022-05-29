# 守护进程

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 实现一个守护进程, 每秒向 `/home/yangdi/a.txt` 递增写入数字.
  - `./main` 使用 `ps axj` 查看当前系统作业信息, 并使用 `tail -f a.txt` 动态查看输出文件.
  - 使用 `kill 进程号` 杀死该进程.

- 知识点:
  - `setsid(2)`
  - `chdir(2)`

- 资料:
  - _(LHQ 29.14)_

## v2

- 代码文件：
  - [main.c](./v2/main.c)

- 说明：
  - 添加系统日志
  - 实现一个守护进程, 每秒向 `/home/yangdi/a.txt` 递增写入数字.
  - `./main` 使用 `ps axj` 查看当前系统作业信息, 并使用 `tail -f /var/log/messages` 动态查看 syslog.
  - 使用 `kill 进程号` 杀死该进程.

- 知识点:
  - `setsid(2)`
  - `chdir(2)`
  - `openlog(3)`
  - `syslog(3)`
  - `closelog(3)`

- 资料:
  - _(LHQ 29.15)_

## v3

- 说明:
  - 加入信号处理, 见 [sigaction(2) 函数的使用 v1](../1124-use-sigaction/README.md#v1)
- 资料:
  - _(LHQ 30.18)_
