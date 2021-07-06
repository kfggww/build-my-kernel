/*
 * 构建内核启动映像
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int copy_bootsect(int, int);
int copy_setup(int, int);
int copy_system(int, int);

int main(int argc, char **argv) {
  if (argc != 5) {
    printf("bootsect setup system hd.img\n");
    return -1;
  }

  // 忽略打开文件错误
  int fd_boot = open(argv[1], O_RDONLY);
  int fd_setup = open(argv[2], O_RDONLY);
  int fd_sys = open(argv[3], O_RDONLY);
  int fd_img = open(argv[4], O_WRONLY);

  // 复制bootsect
  if (copy_bootsect(fd_boot, fd_img))
    return -2;
  printf("复制bootsect完毕!\n");

  // 复制setup
  if(copy_setup(fd_setup, fd_img))
    return -3;
  printf("复制setup完毕!\n");

  // 复制system
  if(copy_system(fd_sys, fd_img))
    return -4;
  printf("复制system完毕!\n");

  close(fd_boot);
  close(fd_setup);
  close(fd_sys);
  close(fd_img);

  return 0;
}

/*
 * 从bootsect复制512字节到img.
 */
int copy_bootsect(int fd_boot, int fd_img) {
  char buf[512] = {0};
  if (read(fd_boot, buf, 512) != 512 ||
      (buf[510] != 0x55 && buf[511] != 0xaa)) {
    printf("bootsect格式错误!\n");
    return -1;
  }
  if (write(fd_img, buf, 512) != 512) {
    printf("写入bootsect错误!\n");
    return -2;
  }
  return 0;
}

/*
 * 复制setup模块到img, setup最大512 * 4字节, 不足填充0.
 */
int copy_setup(int fd_setup, int fd_img) {
  lseek(fd_setup, 0, SEEK_SET);
  lseek(fd_img, 512, SEEK_SET);

  int sectors = 4;
  while (sectors-- > 0) {
    char buf[512] = {0};
    ssize_t size = read(fd_setup, buf, 512);
    if(0 == size)
      break;
    if(-1 == size) {
      printf("读取setup错误!\n");
      return -1;
    }

    if(write(fd_img, buf, 512) != 512) {
      printf("写入setup错误!\n");
      return -2;
    }
  }

  char test_end = 0;
  if(read(fd_setup, &test_end, 1) != 0) {
    printf("setup大小超过4扇区限制!\n");
    return -3;
  }

  return 0;
}


/*复制system模块到img*/
int copy_system(int fd_sys, int fd_img) {
  char buf[512] = {0};
  lseek(fd_sys, 0, SEEK_SET);
  lseek(fd_img, 512 * 5, SEEK_SET);

  ssize_t size = 0;
  while(size = read(fd_sys, buf, 512), size > 0) {
    if(write(fd_img, buf, size) != size) {
      printf("写入system错误!\n");
      return -1;
    }
  }

  return 0;
}
