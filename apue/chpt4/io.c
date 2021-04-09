/* 文件空洞, File Hole.
 * 手动构造一个带有空洞的文件.
 */

#include "../apue.h"

int main(int argc, char **argv) {

  int fd = open("data.txt", O_CREAT|O_WRONLY, S_IREAD|S_IWUSR);
  if(fd == -1) {
    write(STDOUT_FILENO, "open failed\n", 12);
    return -1;
  }

  // 开始写入文件
  char *data = "file with hole";
  const int DATA_SIZE = 14;
  ssize_t count = write(fd, data, DATA_SIZE);
  if(count != DATA_SIZE) {
    write(STDOUT_FILENO, "write failed\n", 13);
    close(fd);
    return -2;
  }

  const int HOLE_SIZE = 1024;
  off_t pos = lseek(fd, HOLE_SIZE, SEEK_END);
  if(pos == -1) {
    write(STDOUT_FILENO, "lseek failed\n", 13);
    close(fd);
    return -3;
  }

  count = write(fd, data, DATA_SIZE);
  if(count != DATA_SIZE) {
    write(STDOUT_FILENO, "write failed in the second time\n", 32);
    close(fd);
  }

  // 验证: 向File Hole中写数据, 且数据长度小于空洞大小, 则文件大小保持不变.
  if(argc > 1) {
    pos = lseek(fd, DATA_SIZE + 1, SEEK_SET);
    if(pos != -1) {
      count = write(fd, data, DATA_SIZE);
      printf("write %zd bytes data into file hole\n", count);
    }
    else
      printf("lseek to file hole failed\n");
  }
  
  close(fd);

  return 0;
}
