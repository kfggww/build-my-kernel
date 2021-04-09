#include "../apue.h"
#include "commons.h"

const char *SERVER_FIFO_ADDR = "/tmp/chpt44_server_fifo";
const char *CLIENT_FIFO_TMPLATE = "/tmp/client.%d";

int main(int argc, char **argv) {

  // 1. 创建服务端FIFO
  if(mkfifo(SERVER_FIFO_ADDR, S_IRUSR|S_IWUSR)) {
    printf("Failed to create server FIFO!");
    return -1;
  }

  // 2. 只读方式打开服务端FIFO. 未指定O_NONBLOCK标志, 此时open调用会阻
  // 塞, 直到有该FIFO有写端打开.
  int server_fifo = open(SERVER_FIFO_ADDR, O_RDONLY);
  if(server_fifo == -1) {
    printf("Failed to open server FIFO!");
    return -2;
  }
  
  // 服务端自己也打开FIFO的写
  open(SERVER_FIFO_ADDR, O_WRONLY);

  int response_num = 0;
  struct request req;
  struct response rsp;

  pid_t server_pid = getpid();
  rsp.server_pid = server_pid;
  
  char client_fifo_name[32] = {0};
  int client_fifo;

  
  // 3.循环处理请求
  while(1) {
    if(read(server_fifo, &req, sizeof(struct request)) != sizeof(struct request)) {
      printf("Failed to read request!");
      continue;
    }

    printf("Request from client[%d] with data: %d\n", req.client_pid, req.request_data);

    sprintf(client_fifo_name, CLIENT_FIFO_TMPLATE, req.client_pid);
    client_fifo = open(client_fifo_name, O_WRONLY);
    if(client_fifo == -1) {
      printf("Failed to open client FIFO!");
      continue;
    }

    rsp.response_data = response_num;
    if(write(client_fifo, &rsp, sizeof(struct response)) != sizeof(struct response)) {
      printf("Failed to write data into client FIFO!");
    }
    close(client_fifo);
    response_num += req.request_data;
  }

  return 0;
}
