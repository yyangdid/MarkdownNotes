# 报式套接字

## v1

- 代码文件
  - [proto.h](./v1/proto.h): 约定
  - [rcver.c](./v1/rcver.c): 接收方, 被动端
  - [snder.c](./v1/snder.c): 发送方, 主动端

- 说明
  - 先运行 `rcver`, 然后换个终端使用 `netstat -anu` 查看.
  - 新终端运行 `snder 127.0.0.1`, 查看 rcver 终端, 可以多运行几次 `snder 127.0.0.1`.

- 知识点:
  - 报式套接字
  - 被动端(先收包, 先运行)
    1. 取得 SOCKET
    2. 给 SOCKET 取得地址, 即: 绑定地址
    3. 收/发消息
    4. 关闭 SOCKET
  - 主动端(先发包)
    1. 取得 SOCKET
    2. 给 SOCKET 取得地址, 即: 绑定地址. (该步骤可省略, 系统会默认给分配一个端口)
    3. 发/收消息
    4. 关闭 SOCKET
  - `socket(2), bind(2), recvfrom(2), recv(2), close(2), htons(3), htonl(3), ntohs(3), ntohl(3)`
  - `inet_pton(3), inet_ntop(3), sendto(2), send(2)`

- 资料:
  - _(LHQ 35.3 35.4)_

## v2

- 代码文件
  - [proto.h](./v2/proto.h): 约定
  - [rcver.c](./v2/rcver.c): 接收方, 被动端
  - [snder.c](./v2/snder.c): 发送方, 主动端

- 说明
  - 修改 [v1](#v1), 变成动态报式套接字传输.
  - 先运行 `rcver`, 然后换个终端使用 `netstat -anu` 查看.
  - 新终端运行 `snder 127.0.0.1 zhangsan`, 查看 rcver 终端, 多运行几次 `snder 127.0.0.1 $NAME$`.

- 知识点:
  - 变长结构体.

- 资料:
  - _(LHQ 35.5)_

## v3

- 代码文件
  - 修改 [v1](#v1), 变成全网广播.
  - [proto.h](./v3/proto.h): 约定
  - [rcver.c](./v3/rcver.c): 接收方, 被动端
  - [snder.c](./v3/snder.c): 发送方, 主动端

- 说明
  - 需先关闭防火墙, 或开放防火墙的 UDP 端口(开放 1989 端口).
  - 先运行 `rcver`, 然后换个终端使用 `netstat -anu` 查看.
  - 新终端运行 `snder`, 查看 rcver 终端.

- 知识点:
  - 全网广播
  - socket 选项
  - `setsockopt(2)` _(LHQ 35.6)_
  - `getsockopt(2)` _(LHQ 35.6)_

- 资料:
  - _(LHQ 35.6 35.7)_

## v4

- 代码文件
  - 修改 [v3](#v3), 变成组播/多播.
  - [proto.h](./v4/proto.h): 约定
  - [rcver.c](./v4/rcver.c): 接收方, 被动端
  - [snder.c](./v4/snder.c): 发送方, 主动端

- 说明
  - 需先关闭防火墙, 或开放防火墙的 UDP 端口(开放 1989 端口).
  - 先运行 `rcver`, 然后换个终端使用 `netstat -anu` 查看.
  - 新终端运行 `snder`, 查看 rcver 终端.

- 知识点:
  - 组播/多播
  - `if_nametoindex(3)`
  - `if_indextoname(3)`

- 资料:
  - _(LHQ 35.7)_

## v5

- 代码文件
  - [proto.h](./v5/proto.h): 约定
  - [rcver.c](./v5/rcver.c): 接收方, 被动端
  - [snder.c](./v5/snder.c): 发送方, 主动端

- 说明
  - 修改 [v1](#v1), 防止数据泄漏.
  - 先运行 `rcver`, 然后换个终端使用 `netstat -anu` 查看.
  - 新终端运行 `snder 127.0.0.1`, 查看 rcver 终端, 可以多运行几次 `snder 127.0.0.1`.

- 知识点:
  - 修改 `snder.c`, 防止内存泄漏, 通过抓包器可以看到有 name 有多余数据被发送.
  - `memset(3)`

- 资料:
  - _(LHQ 35.8)_
