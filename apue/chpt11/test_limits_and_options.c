#include "../apue.h"

int main(int argc, char **argv) {
  long max_login = sysconf(_SC_LOGIN_NAME_MAX);
  printf("LOGIN_NAME_MAX: %ld\n", max_login);

  long max_path = sysconf(_PC_NAME_MAX);
  printf("PC_NAME_MAX: %ld\n", max_path);

  long pipe_buf = sysconf(_PC_PIPE_BUF);
  printf("PC_PIPE_BUF: %ld\n", pipe_buf);

  return 0;
}
