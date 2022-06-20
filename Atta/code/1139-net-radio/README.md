# net radio

- 基于 IPV4 的流媒体广播项目.代码未完成, 只完成到视频 16, 从 17 开始视频好像缺失了.

## 目录及文件说明

- [INSTALL](./v1/INSTALL): 部署指导文件, 如何安装, 简单使用等等.
- [LICENSE](./v1/LICENSE): 使用许可文件.
- [README](./v1/README): 自述文件.
- [Makefile](./v1/Makefile): 主 Makefile.
- [CMakeLists.txt](./v1/CMakeLists.txt): 主 CMakeLists.txt.

- [doc/](./v1/doc/): 文档目录.
  - [admin/](./v1/doc/admin/): 管理员文档目录.
  - [dev/](./v1/doc/dev/): 开发人员文档目录.
  - [user/](./v1/doc/user/): 用户文档目录.(用户使用手册)

- [src/](./v1/src/): 源码目录.
  - [include/](./v1/src/include/): server 端和 client 端的公用头文件, 一般存放的是双方约定的协议.
    - [set_type.h](./v1/src/include/set_type.h): 有关数据类型定义的头文件.
    - [proto.h](./v1/src/include/proto.h): 协议.

  - [client/](./v1/src/client/): client 端源码.
    - [client.h](./v1/src/client/client.h): client 端的头文件.
    - [client.c](./v1/src/client/client.c): client 端主程序.
    - [Makefile](./v1/src/client/Makefile): client 端 Makefile.
    - [CMakeLists.txt](./v1/src/client/CMakeLists.txt): client 端 CMakeLists.txt.

  - [server/](./v1/src/server/): server 端源码.
    - [server.h](./v1/src/server/server.h): server 端的头文件.
    - [server.c](./v1/src/server/server.c): server 端主程序.
    - [thread_list.h](./v1/src/server/thread_list.h): 节目单小模块的头文件.
    - [thread_list.c](./v1/src/server/thread_list.c): 节目单小模块(多线程).
    - [thread_channel.h](./v1/src/server/thread_channel.h): 频道小模块的头文件.
    - [thread_channel.c](./v1/src/server/thread_channel.h.c): 频道小模块(多线程).
    - [media_lib.h](./v1/src/server/media_lib.h): 媒体库相关实现的头文件.
    - [media_lib.c](./v1/src/server/media_lib.c): 媒体库相关实现.
    - [tbf.h](./v1/src/server/tbf.h): 令牌桶流控头文件.
    - [tbf.c](./v1/src/server/tbf.c): 令牌桶流控实现.
    - [Makefile](./v1/src/server/Makefile): server 端 Makefile.
    - [CMakeLists.txt](./v1/src/server/CMakeLists.txt): server 端 CMakeLists.txt.

## v1

- 代码文件：

- 说明：
  - client 为被动端, 先收包, 后运行.(先写)
    - client 命令行选项:
      - `-g` 或 `--group`: 指定多播组 IP.
      - `-p` 或 `--port`: 指定接收端口.
      - `-P` 或 `--player`: 指定播放器命令行.
      - `-h` 或 `--help`: 显示帮助.
  - server 为主动端, 先发包, 先运行, 应该是一个守护进程.
    - server 命令行选项:
      - `-g` 或 `--group`: 指定多播组 IP.
      - `-p` 或 `--port`: 指定接收端口.
      - `-f` 或 `--foreground`: 前台运行.
      - `-d` 或 `--dir`: 指定媒体库位置.
      - `-i` 或 `--interface`: 指定网络设备, 即: 指定发送数据包的网卡.
      - `-h` 或 `--help`: 显示帮助.

- 知识点:

- 资料:
  - _(LHQ 37)_
