# 流式套接字

## v1

- 代码文件：
  - [proto.h](./v1/proto.h): 约定协议.
  - [server.c](./v1/server.c): server 端, 被动端(先收包, 先运行)
  - [client.c](./v1/client.c): client 端, 主动端(先发包, 后运行)

- 说明：
  - 先运行 `server`, 然后换个终端使用 `netstat -ant` 查看.
  - 可在新终端执行 `nc 127.0.0.1 1989` 或 `telnet 127.0.0.1 1989` 测试连接到 server 端.
  - server 端被连接成功时会返回当前时戳.
  - 新终端运行 `client 127.0.0.1`, 查看 server 终端, 可以多运行几次 `client 127.0.0.1`.

- 知识点:
  - 流式套接字.
  - C 端, 主动端(先发包, 后运行)
    1. 获取 SOCKET
    2. 给 SOCKET 取得地址(可省略)
    3. 发起连接
    4. 发/收消息
    5. 关闭 SOCKET
  - S 端, 被动端(先收包, 先运行)
    1. 获取 SOCKET
    2. 给 SOCKET 取得地址
    3. 将 SOCKET 置为监听模式
    4. 接受连接
    5. 收/发消息
    6. 关闭 SOCKET

- 资料:
  - _(LHQ 35.12 35.13 35.14)_

## v2

- 代码文件：
  - [proto.h](./v2/proto.h): 约定协议.
  - [server.c](./v2/server.c): server 端, 被动端(先收包, 先运行)
  - [client.c](./v2/client.c): client 端, 主动端(先发包, 后运行)

- 说明：
  - 修改[v1](#v1)为多进程并发版, 即: 当 server 正在为一个或多个客户端进行服务时, 这是又来了很多客户
    端连接请求, 也能够进行继续连接服务, 即将 对客户端连接的服务 使用多进程并发.
  - 只修改 server.c
  - 先运行 `server`, 然后换个终端使用 `netstat -ant` 查看.
  - 可在新终端执行 `nc 127.0.0.1 1989` 或 `telnet 127.0.0.1 1989` 测试连接到 server 端.
  - server 端被连接成功时会返回当前时戳.
  - 新终端运行 `client 127.0.0.1`, 查看 server 终端, 可以多运行几次 `client 127.0.0.1`.

- 知识点:
  - 流式套接字.
  - server 端多进程并发为多个 client 服务.

- 资料:
  - _(LHQ 35.15)_

## v3

- 代码文件：
  - [proto.h](./v3/proto.h): 约定协议.
  - [server.c](./v3/server.c): server 端, 被动端(先收包, 先运行)
  - [client.c](./v3/client.c): client 端, 主动端(先发包, 后运行)

- 说明：
  - 修改[v1](#v1), 实现 server 端对 client 端的服务使用静态进程池.
  - 只修改 server.c
  - 先运行 `server`, 然后换个终端使用 `netstat -ant` 查看.
  - server 端被连接成功时会返回当前时戳.
  - 新终端运行 `client 127.0.0.1`, 查看 server 终端, 可以多运行几次 `client 127.0.0.1`.

- 知识点:
  - 静态进程池.
  - `accept(2)`: 在并发中天生支持互斥.

- 资料:
  - _(LHQ 35.16 35.17)_

## v4

- 代码文件：
  - [proto.h](./v4/proto.h): 约定协议.
  - [server.c](./v4/server.c): server 端, 被动端(先收包, 先运行)
  - [client.c](./v4/client.c): client 端, 主动端(先发包, 后运行)

- 说明：
  - 修改[v1](#v1), 实现 server 端对 client 端的服务使用动态进程池.
  - 只修改 server.c, 完全重写.
  - 先运行 `server`, 然后换个终端使用 `netstat -ant` 查看.
  - server 端被连接成功时会返回当前时戳.
  - 客户端执行命令: `while true; do (./client 127.0.0.1 &); sleep 1; done`
  - 可以几个终端同时执行上面命令, 具体查看 _(LHQ  36.4)_

- 知识点:
  - 动态进程池.

- 资料:
  - _(LHQ 36.1 36.2 36.3 36.4)_
