#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
  //  open FIFO
  int fd = open("./myfifo", O_WRONLY);
  if(fd == -1) {
    printf("Failed to open FIFO in writer\n");
    return -1;
  }
  // open semaphore
  sem_t *sem_fifo = sem_open("/lock_fifo", O_EXCL, S_IRUSR|S_IWUSR);

  if (argc == 2) {
    char *msg = argv[1];
    sem_wait(sem_fifo);
    printf("do write\n");
    ssize_t count = write(fd, msg, strlen(msg));
    sem_post(sem_fifo);
  } else {
    sem_wait(sem_fifo);
    printf("do write\n");
    write(fd, "Default Message\n", 17);
    sem_post(sem_fifo);
  }

  // close FIFO
  close(fd);

  return 0;
}
