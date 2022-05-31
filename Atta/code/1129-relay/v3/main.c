/**
 * @file main.c
 * @brief 使用有限状态机实现非阻塞 I/O 数据中继在两个 tty 之间传输数据.
 * @version 0.0.3
 * @author yyangdid\@gmail.com
 */

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 两个设备
#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"

// 有限状态机的几种状态
enum {
  STATE_R = 1, // 读取态
  STATE_W,     // 写入态
  // 不存在和使用 STATE_AUTO 态，只相当于一条线，比线小是有条件推动状态机，比线大是无条件推动状态机
  STATE_AUTO,
  STATE_EX, // 异常处理态
  STATE_T   // 进程结束态
};

// 有限状态机数据结构
struct fsm_st {
  int state;         // 当前状态机的状态
  int src_fd;        // 源 文件描述符
  int des_fd;        // 目标 文件描述符
  char buff[BUFSIZ]; // 缓冲区
  ssize_t len;       // 读取, 写入多少个字节
  int pos;           // buff 缓冲区游标位置
  char *err_str;     // 出错字符串
};

// 推动状态机
static void fsm_driver(struct fsm_st *fsm) {
  ssize_t ret; // 写入返回值

  switch (fsm->state) {
  case STATE_R:                                          // 如果是读取态
    fsm->len = read(fsm->src_fd, fsm->buff, BUFSIZ - 1); // 从 src_fd 读取到 buff 缓冲区
    if (fsm->len == 0)                                   // 读取结束
      fsm->state = STATE_T;                              // 去进程结束态
    else if (fsm->len < 0) {                             // 读取失败
      if (errno == EAGAIN)                               // 读取假错
        fsm->state = STATE_R;                            // 回到读取态
      else {                                             // 读取真错
        fsm->err_str = "read()";                         // 读取出错
        fsm->state   = STATE_EX;                         // 去异常态
      }
    } else {                // 读取完成
      fsm->pos   = 0;       // buff 的游标位置
      fsm->state = STATE_W; // 去写入态
    }
    break;
  case STATE_W:                                               // 如果是写入态
    ret = write(fsm->des_fd, fsm->buff + fsm->pos, fsm->len); // 从 buff 缓冲区拿出数据写入到 des_fd
    if (ret < 0) {                                            // 写入失败
      if (errno == EAGAIN)                                    // 写入假错
        fsm->state = STATE_W;                                 // 回到写入态
      else {                                                  // 写入真错
        fsm->err_str = "write()";                             // 写入出错
        fsm->state   = STATE_EX;                              // 去异常态
      }
    } else {
      fsm->pos += (int)ret; // buff 游标移动到已写入的字节位置
      fsm->len -= ret;
      if (fsm->len == 0)      // 写入完成
        fsm->state = STATE_R; // 去读取态
      else                    // 没写完
        fsm->state = STATE_W; // 回到写入态
    }
    break;
  case STATE_EX:          // 如果是异常态
    perror(fsm->err_str); // 报错(是读取出错还是写入出错)
    fsm->state = STATE_T; // 去进程结束态
    break;
  case STATE_T: // 如果是进程结束态
    /* do something... */
    break;
  default:
    /* do something... */
    // 例如:
    abort();
    break;
  }
}

// 数据中继引擎
static void relay(int fd1, int fd2) {
  // 定义状态机
  struct fsm_st fsm_r1_w2, fsm_r2_w1;

  // 将两个文件描述符都做成非阻塞的
  // (调用该函数时两个文件描述符可能是阻塞的,该函数执行完成之前记得要恢复两个文件描述符的原状态)
  int fd1_save;
  fd1_save = fcntl(fd1, F_GETFL); // 获取 fd1 文件描述符的属性
  // 设置 fd1 文件描述符的属性, 给它增加设置非阻塞属性
  // (若本身就是非阻塞, 再给它设置非阻塞也没问题, 它的非阻塞属性不变)
  fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);

  int fd2_save;
  fd2_save = fcntl(fd2, F_GETFL);
  fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);

  // 初始化状态机状态
  fsm_r1_w2.state  = STATE_R; // 设置状态机 读 1 写 2 的状态为读取态
  fsm_r1_w2.src_fd = fd1;
  fsm_r1_w2.des_fd = fd2;
  fsm_r2_w1.state  = STATE_R;
  fsm_r2_w1.src_fd = fd2;
  fsm_r2_w1.des_fd = fd1;

  // 当有一个或两个状态机的状态不是进程结束态时
  struct pollfd pfd[2];
  pfd[0].fd = fd1;
  pfd[1].fd = fd2;
  while (fsm_r1_w2.state != STATE_T || fsm_r2_w1.state != STATE_T) {
    // 布置监视任务
    pfd[0].events = 0;              // 位图需要清零
    if (fsm_r1_w2.state == STATE_R) // 若状态机 1 的状态为读，那 fd1 可读
      pfd[0].events |= POLLIN;
    if (fsm_r2_w1.state == STATE_W) // 若状态机 2 的状态为写，那 fd1 可写
      pfd[0].events |= POLLOUT;

    pfd[1].events = 0;              // 位图需要清零
    if (fsm_r1_w2.state == STATE_W) // 若状态机 1 的状态为写，那 fd2 可写
      pfd[1].events |= POLLOUT;
    if (fsm_r2_w1.state == STATE_R) // 若状态机 2 的状态为读，那 fd2 可读
      pfd[1].events |= POLLIN;

    // 监视
    // 当状态机状态为读取态或写入态时
    if (fsm_r1_w2.state < STATE_AUTO || fsm_r2_w1.state < STATE_AUTO) {
      while (poll(pfd, 2, -1) < 0) { // 以阻塞形式进行监视
        if (errno == EINTR)          // 假错，被信号打断
          continue;
        perror("poll()"); // 真错
        exit(1);
      }
    }

    // 查看监视结果
    // 根据监视结果，有条件的推动状态机
    // 若 fd1 可读或 fd2 可写或状态机 1 的状态为异常态或进程结束态
    if (pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT || fsm_r1_w2.state > STATE_AUTO)
      fsm_driver(&fsm_r1_w2); // 推动状态机
    if (pfd[1].revents & POLLIN || pfd[0].revents & POLLOUT || fsm_r2_w1.state > STATE_AUTO)
      fsm_driver(&fsm_r2_w1);
  }

  // 恢复文件描述符的原状态
  fcntl(fd1, F_SETFL, fd1_save);
  fcntl(fd2, F_SETFL, fd2_save);
}

int main() {
  int fd1, fd2;             // 文件描述符
  fd1 = open(TTY1, O_RDWR); // 以阻塞的读写方式打开 TTY1
  if (fd1 < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  write(fd1, "MY_TTY1\n", 8); // 终端提示语句

  fd2 = open(TTY2, O_RDWR | O_NONBLOCK); // 以非阻塞的读写方式打开 TTY2
  if (fd2 < 0) {
    fprintf(stderr, "%s->%d Error: %s\n", __FILE__, __LINE__, strerror(errno));
    fflush(NULL);
    exit(EXIT_FAILURE);
  }
  write(fd2, "MY_TTY2\n", 8); // 终端提示语句

  relay(fd1, fd2); // 数据中继引擎函数

  close(fd2);
  close(fd1);

  exit(EXIT_SUCCESS);
}
