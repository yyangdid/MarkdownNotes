/**
 * @file main.c
 * @brief shell 实现.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

// 分隔符
#ifndef DELIMITS
  #define DELIMITS " \t\n"
#endif

struct cmd_st {
  glob_t glob_rest;
};

// 打印提示符
static void prompt(void) {
  printf("myshell$ ");
}

// 解析命令
static void parse(char *line, struct cmd_st *rest) {
  char *tok;
  int i = 0;
  while (1) {
    // 命令行输入分解成一个个短字符串, 即: argv[0], argv[1], argv[2] ...
    tok = strsep(&line, DELIMITS);
    if (tok == NULL)
      break;
    if (tok[0] == '\0')
      continue;
    // 将短字符串存储到 glob_t 类型中, 只有第一次不追加, 以后每次都追加.
    // 位图运算中乘法运算优先级高于或运算
    glob(tok, GLOB_NOCHECK | GLOB_APPEND * i, NULL, &rest->glob_rest);
    i = 1;
  }
}

int main() {
  while (1) {
    prompt(); // 打印提示符

    char *line_buff       = NULL;
    size_t line_buff_size = 0;
    if (getline(&line_buff, &line_buff_size, stdin) < 0)
      break;

    struct cmd_st cmd;
    parse(line_buff, &cmd); // 解析
    if (0) { // 内部命令 // 该程序暂时只处理外部命令, 内部命令不进行处理
      // do ...
    } else { // 外部命令
      pid_t pid = fork();
      if (pid < 0) { // 出错
        fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                strerror(errno));
        fflush(NULL);
        exit(EXIT_FAILURE);
      }
      if (pid == 0) { //子进程
        execvp(cmd.glob_rest.gl_pathv[0], cmd.glob_rest.gl_pathv);
        fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__,
                strerror(errno));
        fflush(NULL);
        exit(EXIT_FAILURE);
      } else { // 父进程
        wait(NULL);
      }
    }
  }

  exit(EXIT_SUCCESS);
}
