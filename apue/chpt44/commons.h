#ifndef COMMONS_H_
#define COMMONS_H_
#include <unistd.h>

struct request {
  pid_t client_pid;
  int request_data;
};

struct response {
  pid_t server_pid;
  int response_data;
};

#endif
