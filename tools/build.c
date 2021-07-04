/*
 * 构建内核启动映像
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int copy_bootsect(int, int);

int main(int argc, char **argv) {
  if (argc != 5) {
    printf("bootsect setup system hd.img\n");
    return -1;
  }

  // 忽略打开文件错误
  int boot_fd = open(argv[1], O_RDONLY);
  int img_fd = open(argv[4], O_WRONLY);

  // 复制bootsect
  if (copy_bootsect(boot_fd, img_fd))
    return -2;
  printf("复制bootsect完毕!\n");

  close(boot_fd);
  close(img_fd);

  return 0;
}

/*
 * 从bootsect复制512字节到img.
 */
int copy_bootsect(int boot_fd, int img_fd) {
  char buf[512] = {0};
  if (read(boot_fd, buf, 512) != 512 ||
      (buf[510] != 0x55 && buf[511] != 0xaa)) {
    printf("bootsect格式错误!\n");
    return -1;
  }
  if (write(img_fd, buf, 512) != 512) {
    printf("写入img出错!\n");
    return -2;
  }
  return 0;
}
