#include "../apue.h"

// 打开一个terminal, 多次运行./test命令, 得到的session id都是同一个值, 当打开另
// 一个terminal, 再次执行一样的操作之后, 得到的session id是和之前不同的值.
int main() {
    pid_t sid = getsid(0);
    printf("current session id: %d\n", sid);
    return 0;
}
