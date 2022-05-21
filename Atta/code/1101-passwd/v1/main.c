/**
 * @file main.c
 * @brief 根据 uid 获取对应的用户名.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s uid\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  struct passwd *pwd_line;
  errno    = 0;
  pwd_line = getpwuid(strtol(argv[1], NULL, 10));
  if (pwd_line == NULL) {
    if (!errno) {
      printf("不存在.\n");
      fflush(NULL);
      exit(EXIT_SUCCESS);
    }
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  printf("%s\n", pwd_line->pw_name);

  exit(EXIT_SUCCESS);
}
