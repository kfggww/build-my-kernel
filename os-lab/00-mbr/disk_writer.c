// 从硬盘文件的一个扇区开始, 写入另一个文件的内容
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#define SECTOR_SIZE 512

int main(int argc, char **argv) {
  if(argc != 4)
    return -1;

  char * src_file = argv[1];
  char * hd_file = argv[2];
  int start_id = atoi(argv[3]);

  int fd_src = open(src_file, O_RDONLY);
  if(fd_src < 0)
    return -2;

  int fd_hd = open(hd_file, O_WRONLY);
  if(fd_hd < 0)
    return -3;

  if(lseek(fd_hd, start_id * SECTOR_SIZE, SEEK_SET) < 0)
    return -4;

  char *buf[SECTOR_SIZE] = {0};
  while(read(fd_src, buf, SECTOR_SIZE) > 0) {
    write(fd_hd, buf, SECTOR_SIZE);
  }

  printf("Successfully copy %s to sector %d of %s!\n", src_file, start_id, hd_file);
  close(fd_src);
  close(fd_hd);

  return 0;
}
