#include "../apue.h"
#include <time.h>

int main(int argc, char **argv) {
  time_t sec = time(NULL);
  char *date = ctime(&sec);
  printf("Current time: %s\n", date);
  return 0;
}
