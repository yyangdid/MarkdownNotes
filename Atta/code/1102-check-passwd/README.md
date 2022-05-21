# 校验用户密码

## v1

- 代码文件：
  - [main.c](./v1/main.c)

- 说明：
  - 运行: `./main zhangsan`
  - 说明: 该程序需使用 root 用户执行, 因为需要在 `/etc/shadow` 文件中读取, 只有 root 能读取

- 知识点:
- `getspnam(3)`
- `getspent(3)`
- `crypt(3)`
  - 需添加编译参数 `-lcrypt`
- `getpass(3)`

- 资料:
  - _(LHQ 28.7)_
