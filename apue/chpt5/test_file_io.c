// 以O_APPEND打开文件, 然后lseek到文件开始, 再次写入数据到文件, 观察写
// 入文件的数据的位置.
#include "../apue.h"

int main(int argc, char **argv) {

  const char *file_name = "/tmp/data.txt";
  int fd = open(file_name, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
  write(fd, "test", 4);
  lseek(fd, 0, SEEK_SET);
  // 当使用O_APPEND打开文件时, 每次write操作, 内核都会先定位到文件结尾,
  // 然后写入数据, 并且这个操作是原子的.
  write(fd, "test", 4);
  close(fd);
  return 0;
}
