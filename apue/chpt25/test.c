// 混合使用syscall和标准库进行IO操作时可能出现一些意想不到的情况, 当把进程的标准
// 输出重定向到一个文件之后和不做重定向, 得到的输出结果是不同的. 原因如下:
// printf这个库函数使用的缓冲区在用户空间, 而write系统调用直接使用内核空间的缓冲
// 区, 并且fork会复制父进程的fd表和数据, 当进程退出执行exit库函数时, 用户空间的
// 缓冲区内容会被刷新到内核空间, 进而输出到文件
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    printf("Hello world\n");
    write(STDOUT_FILENO, "Bye\n", 4);

    if(fork() == -1)
        return -1;

    // 使用两种退出方式得到的结果是不同的(在重定向输出的文件的情况下), 由此可见,
    // 库函数exit和系统调用_exit做了不一样的工作
    // _exit(0);
    return 0;
}
