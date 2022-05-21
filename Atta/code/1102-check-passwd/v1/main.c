/**
 * @file main.c
 * @brief 校验用户密码.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s UserName\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 输入密码
  char *input_pass;
  input_pass = getpass("PassWord:");
  if (input_pass == NULL) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 获取需要校验密码的用户在 /etc/shadow 文件中的一行记录, 为的是获取该用户密码的加密方式和杂字串
  struct spwd *shadow_line;
  shadow_line = getspnam(argv[1]);
  if (shadow_line == NULL) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 根据从 /etc/shadow 获取的该用户的加密方式和杂字串对输入的密码进行加密
  // crypt(3) 第二个参数只能读取到第一个 $ 到第三 $ 之间的内容(包括三个 $), 忽略第三个 $ 后的内容
  char *encrypted_pass;
  encrypted_pass = crypt(input_pass, shadow_line->sp_pwdp);

  // 将 encrypted_pass 和 /etc/shadow 中存储的用户加密口令进行对比
  if (strcmp(shadow_line->sp_pwdp, encrypted_pass) == 0)
    printf("密码正确!\n");
  else
    printf("密码错误!\n");

  exit(EXIT_SUCCESS);
}
