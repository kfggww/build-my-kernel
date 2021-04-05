#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int fd[2];
  int err = pipe(fd);
  if (err == -1)
    printf("Failed to open pipe!");

  pid_t pid = fork();

  if (pid == 0) {
    close(fd[0]);
    write(fd[1], argv[1], strlen(argv[1]) + 1);
  } else if (pid > 0) {
    close(fd[1]);
    char buf[1024];
    read(fd[0], buf, 1024);
    printf("Read data from pipe: %s\n", buf);
  }

  return 0;
}
