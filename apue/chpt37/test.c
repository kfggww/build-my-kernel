#include "../apue.h"
#include <stdlib.h>

int main() {

    // 第一次fork
    switch(fork()){
        case -1:
            printf("第一次fork失败!\n");
            exit(-1);
        case 0:
            // 新建session
            if(setsid() == -1) {
                printf("创建session失败!\n");
                exit(-2);
            }

            // 第二次fork
            switch(fork()) {
                case -1:
                    printf("第二次fork失败!\n");
                    exit(-3);

                case 0:
                    // 关闭不需要的fd
                    close(STDIN_FILENO);
                    close(STDOUT_FILENO);
                    close(STDERR_FILENO);
                    // 切换进程的工作目录到"/", 避免出现文件系统不能卸载的
                    if(chdir("/") != 0)
                        exit(-4);
                    // 执行新的程序, 这里是真正在后台完成工作的地方
                    execve("/tmp/daemon_demo", NULL, NULL);
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return 0;
}
