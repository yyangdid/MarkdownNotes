/**
 * @file main.c
 * @brief 解析目录(非递归).
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MY_PATTERN
  // 必须要有 `*`, 因为要的是 pattern, 必须有通配符
  // `/etc/*` 不包含隐藏文件, `/etc/.*` 是只显示隐藏文件
  // 所有文件暂时还不知道怎么搞
  #define MY_PATTERN "/etc/*"
#endif

/*
 * pattern 解析失败函数(glob(3) 函数的第三个参数).
 * 这里只是写出了失败函数, 下面的 glob(3) 里面暂时没有使用.
 */
// static int err_func(const char *err_path, int errno) {
//   printf("%s\n", err_path);
//   fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
//   fflush(NULL);
//   return 0;
// }

int main() {
  glob_t glob_rest;
  int err = glob(MY_PATTERN, 0, NULL, &glob_rest);
  if (err) {
    // GLOB_NOMATCH 为没有匹配项(即: 目录为空或要匹配的文件不存在, 正常现象, 为假错误)
    if (err == GLOB_NOMATCH) {
      fprintf(stdout, "Not found matches!\n");
    } else {
      fprintf(stderr, "%s->%s()->%d Error\n", __FILE__, __FUNCTION__, __LINE__);
    }
    globfree(&glob_rest);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // for (int i = 0; glob_rest.gl_pathv[i] != NULL; i++) {
  for (int i = 0; i < glob_rest.gl_pathc; i++) {
    printf("%s\n", glob_rest.gl_pathv[i]);
  }

  globfree(&glob_rest);

  exit(EXIT_SUCCESS);
}
