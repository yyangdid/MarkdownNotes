# 解析命令行参数

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 运行:
    - `./main`
    - `./main -y 4`
    - `./main -y 2`
    - `./main -m`
    - `./main -y 2 -m -d`
    - `./main -m -d -y 4 -H 12`
    - `./main -y 4 -m -d -H 24 -M -S`
    - `./main -ymd 4` 会报错, 带有选项的参数必须单独写或放在连写参数的最后一个
    - `./main -mdMSy 4`
    - `./main -y 4 -mdH 24 -MS`
    - `./main -y 4 -m -d /etc/shadow` 在非 root 用户下一定写入文件失败, 因为没权限, 只能输出到终端
    - `./main -y 4 -mdH 24 -MS ~/d.txt`
    - `./main -y 4 -m ~/d1.txt -dH 24 ~/d2.txt -MS ~/d3.txt` 只有 ~/d1.txt 非选项传参有效
  - 该程序根据给出的命令行参数显示日期时间信息.
    - `-y` : year 后面必须跟 2 或 4 作为选项
    - `-m` : month
    - `-d` : day
    - `-H` : hour 后面必须跟 12 或 24 作为选项
    - `-M` : minute
    - `-S` : second
  - 程序中 `-y` 和 `-H` 后面跟的数字是参数的选项, 而 `ls -a /etc/` 中的 `/etc/` 不属于参数的选项, 这
    是非选项的参数(即: 非选项传参).
  - 程序允许使用一个文件路径作为非选项传参, 会将要求的日期时间输出到该文件中(若文件存在则覆盖, 不存
    在则创建)
  - 若给定了多个文件路径作为非选项传参, 那么一般会有三种处理方式
    - 前效(只有第一个有效文件路径有效)(本程序使用的)
    - 后效(只有最后一个有效文件路径有效)(比较常见)
    - 全效(所有有效文件路径都有效, 给每个文件都写入一次)
  - 该程序修改自: [获取一百天以后的日期 v1](../1104-get-100day-later/README.md#v1)

- 知识点:
  - `getopt(3)`
    - `optarg` (man 3 getopt)
    - `optind` (man 3 getopt)
    - `opterr` (man 3 getopt)
    - `optopt` (man 3 getopt)
  - `fopen(3)`
  - `fclose(3)`
  - `time(2)`
  - `localtime(3)`
  - `strftime(3)`

- 资料:
  - _(LHQ 28.13 28.14)_
