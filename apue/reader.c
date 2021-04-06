#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // make a FIFO in current working directory named "myfifo"
  // open a semaphore
  sem_t *sem_fifo = sem_open("/lock_fifo", O_CREAT, S_IRUSR | S_IWUSR, 1);

  // read data from FIFO
  int fd = open("./myfifo", O_RDONLY);
  if (fd == -1) {
    printf("Failed to open FIFO\n");
    return -2;
  }

  int read_count = 10;
  char buf[1024];
  while (1) {
    ssize_t count = read(fd, buf, 1024);
    if (count > 0)
      printf("Read data of [%zd]: %s\n", count, buf);
  }

  return 0;
}
