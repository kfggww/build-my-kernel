#include "../apue.h"
#include "commons.h"

const char *SERVER_FIFO_ADDR = "/tmp/chpt44_server_fifo";
const char *CLIENT_FIFO_TMPLATE = "/tmp/client.%d";

int main(int argc, char **argv) {

  pid_t client_pid = getpid();
  struct request req = {client_pid, 0};

  if (argc == 1)
    req.request_data = 0;
  else
    req.request_data = (int)argv[1][0];

  // 1. 打开客户端接受响应的FIFO
  char client_fifo_name[32] = {0};
  sprintf(client_fifo_name, CLIENT_FIFO_TMPLATE, client_pid);
  if (mkfifo(client_fifo_name, S_IRUSR | S_IWUSR) == -1) {
    printf("Failed to create client FIFO!");
    return -1;
  }

  // 2. 只写方式打开服务端FIFO, 用于发送请求
  int server_fifo_fd = open(SERVER_FIFO_ADDR, O_WRONLY);
  if (server_fifo_fd == -1) {
    printf("Failed to open server FIFO!");
    return -2;
  }

  // 3. 发送请求数据
  if (write(server_fifo_fd, &req, sizeof(struct request)) !=
      sizeof(struct request)) {
    printf("Failed to send request!");
    close(server_fifo_fd);
    return -3;
  }
  close(server_fifo_fd);

  // 3. 只读方式打开客户端FIFO
  int client_fifo_fd = open(client_fifo_name, O_RDONLY);
  if (client_fifo_fd == -1) {
    printf("Failed to open client FIFO in client[%d]!", client_pid);
    return -4;
  }

  struct response rsp;
  if (read(client_fifo_fd, &rsp, sizeof(struct response)) !=
      sizeof(struct response)) {
    printf("Failed to read response in client[%d]!", client_pid);
    close(client_fifo_fd);
    return -5;
  }

  printf("client[%d] get response from server[%d] with data: %d\n", client_pid,
         rsp.server_pid, rsp.response_data);
  close(client_fifo_fd);
  return 0;
}
