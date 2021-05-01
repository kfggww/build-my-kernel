// 使用pipe实现进程间通信

#include <unistd.h>

int main() {

    int pfd[2];
    if(pipe(pfd) != 0)
        _exit(-1);

    if(fork() == 0) {
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        execlp("ls", "ls", (char *)NULL);
        close(pfd[1]);
        _exit(0);
    }

    if(fork() == 0) {
        close(pfd[1]);
        dup2(pfd[0], STDIN_FILENO);
        execlp("wc", "wc", (char *)NULL);
        _exit(0);
    }

    close(pfd[0]);
    close(pfd[1]);

    return 0;
}
