# 创建一个空洞文件

## v1

- 代码文件：

  - [main.c](./v1/main.c)

- 说明：

  - 运行：`./main ~/zzz`

- 知识点：

  - 文件大小和文件所占的磁盘大小不一样
  - 查看文件的实际大小: `ls -l XXX` `ls -lh XXX`
  - 查看文件占用磁盘空间大小: `du -h XXX`
  - 查看文件详细信息: `stat XXX`
  - `Block` 扇区、硬扇区、设备块, 一般为 512 Byte, 设备的最小寻址单元
  - `Blocks`: 文件所占的 `Block` 数
  - `IO Block`: 文件块、 I/O 块, 文件系统的最小寻址单元
  - `Links` : 表示硬链接数
  - 详见: [如何理解 stat 命令显示的 Blocks 和 IO Block](https://blog.csdn.net/lyndon_li/article/details/123984482)

- 知识点二:
  - `cp` 命令是支持空洞文件的
    - 用 `ls -l ~/zzz` 和 `stat ~/zzz` 查看创建文件的信息
    - 用 `cp ~/zzz ~/zzz.101.bak` 做个文件的拷贝
    - 用 `ls -l ~/zzz.101.bak` 和 `stat ~/zzz.101.zzz` 会发现该文件不占用磁盘空间, `Blocks` 为 0.

- 资料：
  - _(LHQ 27.3)_
