// How to use pipe
/* 1. 为何进程需要关闭自己不用的fd?
 *    - 为了避免可能发生的死锁
 *    - 为了释放管道相关的资源
 */

#include "apue.h"

/**
 * 测试在一个进程中打开一个pipe, 不关闭写端, 然后尝试读取, 是否会导致
 * 死锁.
 */
void case_01() {
  int pipefd[2];
  int err = pipe(pipefd);
  if (err == -1) {
    printf("打开pipe失败!");
    return;
  }

  // 若不关闭管道写端, 内核知道当前管道还有一个写端, read的时候会阻塞
  // 直到有新数据写入, 但这个写端是在当前进程内打开的, 永远不会有数据
  // 通过这个写端写数据到管道, 结果就是死锁, 当前进程在自己等自己, 这
  // 也可以说明, 只有一个进程也可以导致死锁.
  close(pipefd[1]);

  // 直接读取管道数据
  char buf[1024] = {0};
  ssize_t read_count = read(pipefd[0], buf, 1023);
  printf("读取数据长度: %zd bytes, 读取数据内容: %s\n", read_count, buf);

  return;
}

/**
 * 写进程, 如果不关闭自己的读端.
 */
void case_02() {
  int pipefd[2];
  int err = pipe(pipefd);
  if (err == -1) {
    printf("打开pipe失败!");
    return;
  }

  // 关闭写进程的读端, 当前管道只有写端, 这是一个broken pipe, 在其上写
  // 入会失败.
  close(pipefd[0]);

  // 如果不关当前写进程的读端, 同样可能会导致死锁的发生

  ssize_t write_count = write(pipefd[1], "hello", 5);
  printf("写入数据长度: %zd bytes\n", write_count);
}

/**
 * 读取命令行参数, 通过管道写到子进程, 子进程再把受到的内容写到标注输
 * 出.
 */
int case_03(int argc, char **argv) {

  if (argc == 1) {
    printf("用法: %s 参数\n", argv[0]);
    return -1;
  }

  int fd[2];
  if (pipe(fd) == -1) {
    printf("打开pipe失败!");
    return -2;
  }

  char buf[10];
  // 创建子进程
  // 关闭不需要的fd, 必须在合适的位置, 如果在创建子进程之前就把fd[0]关
  // 了, 那么fork出的子进程, fd[0]也是关闭的.
  switch (fork()) {
  case 0: // child
    close(fd[1]);
    read(fd[0], buf, 10);
    close(fd[0]);
    write(STDOUT_FILENO, buf, 10);
    write(STDOUT_FILENO, "\n", 1);
    break;
  case -1:
    printf("创建子进程失败!");
    return -3;
  default: // parent
    close(fd[0]);
    write(fd[1], argv[1], 10);
    int wsstatus;
    wait(&wsstatus);
    close(fd[1]);
    break;
  }
  return 0;
}

int main(int argc, char **argv) {

  /* case_01(); */
  /* case_02(); */
  case_03(argc, argv);
  return 0;
}
