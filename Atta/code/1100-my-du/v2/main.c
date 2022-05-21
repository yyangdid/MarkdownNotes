/**
 * @file main.c
 * @brief 解析目录(递归).
 * @version 0.0.2
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef PATH_SIZE
  #define PATH_SIZE (1024)
#endif

// 用于判断一个路径是否是死循环(即: 隐藏的 . 和 .. 目录文件为死循环路径)
static int path_isNoLoop(const char *path) {
  // 查看一个完成路径的最后一个 / 后面是否为完整的 . 或 ..
  char *pos = strrchr(path, '/'); // 查找最后一个 / 字符的位置
  if (pos == NULL) {              // 若没找到
    exit(EXIT_FAILURE);
  }
  if (strcmp(pos + 1, ".") == 0 || strcmp(pos + 1, "..") == 0) {
    return 0;
  }
  return 1;
}

static int64_t my_du(const char *path) {
  // 判断 path 为目录文件还是非目录文件
  static struct stat stat_rest;
  if (lstat(path, &stat_rest) < 0) { // 这里使用 lstat(2) 是为了不展开符号链接文件
    fprintf(stderr, "%s->%s()->%d Error: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 若 path 为非目录文件
  if (!S_ISDIR(stat_rest.st_mode)) {
    return stat_rest.st_blocks; // 返回 Block 数
  }

  // 若 path 为目录文件
  static char next_path[PATH_SIZE] = "";
  // 为 path 目录追加 /*, 以便于下面进行解析
  strncpy(next_path, path, PATH_SIZE - 1);
  // 注意: strncpy(3) 不会自动追加尾零, 若字符数组定义时未初始化为空, 则需手动追加尾零
  // next_path[PATH_SIZE - 1] = '\0';
  strncat(next_path, "/*", PATH_SIZE - 1); // strncat(3) 会自动追加尾零

  glob_t glob_rest;
  // 解析 path 下所有非隐藏文件
  int err_glob = glob(next_path, 0, NULL, &glob_rest);
  if (err_glob) {
    // GLOB_NOMATCH 为没有匹配项(即: 目录为空或要匹配的文件不存在, 正常现象, 为假错误)
    if (err_glob != GLOB_NOMATCH) {
      globfree(&glob_rest);
      fprintf(stderr, "%s->%s->%d Error\n", __FILE__, __FUNCTION__, __LINE__);
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  // 解析 path 下所有非隐藏文件
  strncpy(next_path, path, PATH_SIZE - 1);
  strncat(next_path, "/.*", PATH_SIZE - 1);
  err_glob = glob(next_path, GLOB_APPEND, NULL, &glob_rest); // 将解析结构追加 glob_rest 中
  if (err_glob) {
    // GLOB_NOMATCH 为没有匹配项(即: 目录为空或要匹配的文件不存在, 正常现象, 为假错误)
    if (err_glob != GLOB_NOMATCH) {
      globfree(&glob_rest);
      fprintf(stderr, "%s->%s->%d Error\n", __FILE__, __FUNCTION__, __LINE__);
      fflush(NULL);
      exit(EXIT_FAILURE);
    }
  }

  int64_t sum = stat_rest.st_blocks;
  for (int i = 0; i < glob_rest.gl_pathc; i++) {
    // 隐藏文件包括 . 和 .. 即:当前路径和上层路径, 解析时这两个都是死循环(实际死在一直无限循环
    // 解析 .(当前路径) 上了, 根本进不了 ..(上层路径)), 需要排除这两个隐藏的目录文件
    if (path_isNoLoop(glob_rest.gl_pathv[i]))
      sum += my_du(glob_rest.gl_pathv[i]); // 递归
  }

  // 追加当前路径的文件大小
  // 被优化到上面 sum 定义初始化的地方了
  // sum += stat_rest.st_blocks;

  globfree(&glob_rest);

  return sum; // 返回 BLock 数
}

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--hep") == 0 || strcmp(argv[1], "-h") == 0) {
    fprintf(stderr,
            "Wrong number of parameters!\n"
            "Usage: %s file\n",
            argv[0]);
    fflush(NULL);
    exit(EXIT_FAILURE);
  }

  // 一般每个 Block 为 512Byte, 除以 2 就是 KB
  printf("%ld KB\n", my_du(argv[1]) / 2);

  exit(EXIT_SUCCESS);
}
