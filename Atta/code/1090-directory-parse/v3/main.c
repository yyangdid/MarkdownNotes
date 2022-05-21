/**
 * @file main.c
 * @brief 解析目录(非递归).
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MY_PATH
  #define MY_PATH "/etc"
  // 或
  // #define MY_PATH "/etc/"
#endif

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  DIR *dp = opendir(MY_PATH);
  if (dp == NULL) {
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  struct dirent *cur;
  while ((cur = readdir(dp)) != NULL) {
    printf("%s\n", cur->d_name);
  }

  closedir(dp);

  exit(EXIT_SUCCESS);
}
