# XSI 通信示例

## v1

- 代码文件：
  - [proto.h](./v1/proto.h): 约定协议.
  - [snder.c](./v1/snder.c): 发送方(只管发送,需手工编译, `gcc snder.c -o snder`).
  - [rcver.c](./v1/rcver.c): 接收方(先运行, 也先写, 只管接收, 需手工编译: `gcc rcver.c -o rcver`).

- 说明：
  - XSI ipc 消息队列示例, 无亲缘关系进程间的通信.
  - 先运行: `./rcver`, 然后用 `ipcs` 进行查看. 再运行: `./snder`.
  - 先运行: `./snder` 几次, 然后运行 `./rcver`
    - 发现所有消息居然收到了, 消息队列有缓存消息的能力, 使用 `ulimit -a` 查看可缓存的数据量大小.
  - 程序只能强制退出异常终止, 使用 `ipcs` 查看发现消息队列居然还在, 需使用 `ipcrm`
    命令(`ipcrm -q XXmsqidXX`, 例如 `ipcrm -q 0`)进行删除.

- 知识点:
  - `Message Queues`: 消息队列.

- 资料:
  - _(LHQ 34.3 34.4)_
