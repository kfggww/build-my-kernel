#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#define SIZE 510

int main(int argc, char **argv) {

  int fd_bin = open("a.bin", O_RDONLY);
  if (fd_bin < 0) {
    printf("Failed to open file a.bin!\n");
    return -1;
  }

  int fd_img = open("hd.img", O_WRONLY);
  if (fd_img < 0) {
    printf("Failed to open file hd.img!\n");
    return -1;
  }

  char buf[SIZE] = {0};
  ssize_t read_size = read(fd_bin, buf, SIZE);
  if (read_size < 0) {
    printf("Failed to read file a.bin!\n");
    close(fd_bin);
    return -2;
  }
  close(fd_bin);

  if (write(fd_img, buf, SIZE) != SIZE) {
    printf("Failed to write file a.bin to STDOUT!\n");
    close(fd_img);
    return -3;
  }

  buf[0] = 0x55;
  buf[1] = 0xaa;
  if (write(fd_img, buf, 2) != 2) {
    close(fd_img);
    printf("Failed to write MBR tail to STDOUT!\n");
    return -4;
  }

  close(fd_img);

  return 0;
}
