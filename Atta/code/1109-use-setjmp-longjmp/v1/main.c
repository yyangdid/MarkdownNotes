/**
 * @file main.c
 * @brief setjmp 和 longjmp 的使用.
 * @version 0.0.1
 * @author yyangdid\@gmail.com
 */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf jmp_save;

static void d(void) {
  printf("%s(): Begin.\n", __FUNCTION__);

  printf("%s(): Jump now!\n", __FUNCTION__);
  longjmp(jmp_save, 6);
  // longjmp(jmp_save, 0); // 第二个参数必须为非 0 值, 若给个 0, 该函数会自动更正为 1

  printf("%s(): End.\n", __FUNCTION__); // 上面使用长跳转后, 这句永远不会被执行
}

static void c(void) {
  printf("%s(): Begin.\n", __FUNCTION__);
  printf("%s(): Call d().\n", __FUNCTION__);
  d();
  printf("%s(): d() returned.\n", __FUNCTION__);
  printf("%s(): End.\n", __FUNCTION__);
}

static void b(void) {
  printf("%s(): Begin.\n", __FUNCTION__);
  printf("%s(): Call c().\n", __FUNCTION__);
  c();
  printf("%s(): c() returned.\n", __FUNCTION__);
  printf("%s(): End.\n", __FUNCTION__);
}

static void a(void) {
  printf("%s(): Begin.\n", __FUNCTION__);

  int ret = setjmp(jmp_save);
  if (ret == 0) { // 是设置跳转点
    printf("%s(): Call b().\n", __FUNCTION__);
    b();
    printf("%s(): b() returned.\n", __FUNCTION__);
  } else { // 是从其他地方跳回来的
    printf("%s(): Jumped back here whith code %d\n", __FUNCTION__, ret);
  }

  printf("%s(): End.\n", __FUNCTION__);
}

int main() {
  printf("%s(): Begin.\n", __FUNCTION__);
  printf("%s(): Call a().\n", __FUNCTION__);
  a();
  printf("%s(): a() returned.\n", __FUNCTION__);
  printf("%s(): End.\n", __FUNCTION__);

  exit(EXIT_SUCCESS);
}
