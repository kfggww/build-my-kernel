// chapter 26 exercises
#include "../apue.h"


// 验证父进程终止之后, 子进程的父变成了init
// 不明白为何子进程在打印完毕之后, 需要点击回车, shell才可以显示提示符
void test26_1() {

    switch(fork()) {
        case -1:
            printf("Failed to fork a child process!\n");
            _exit(2);

        case 0:
            sleep(3);
            pid_t ppid = getppid();
            pid_t pid = getpid();
            printf("After old father dead, new father is: %d, current process is: %d\n", ppid, pid);
            _exit(0);

        default:
            printf("Old father dead.\n");
            _exit(0);
    }
}

// grandparent, parent, child三个进程之间的关系. parent process 终止之后成为僵尸
// 进程, 但依旧是child的父进程, 直到grandparent调用了wait之后, 成为僵尸进程的
// parent进程从内核中被销毁, child才被init领养.
void test26_2() {
    pid_t parent_pid, pid;
    switch(fork()) {
        case -1:
            printf("Failed to fork parent!\n");
            _exit(1);

        case 0:
            parent_pid = getpid();
            printf("Fork parent process, %d\n", parent_pid);
            if(fork() == 0) { // child
                while(getppid() != 1) {
                    sleep(2);
                }
                printf("child has been adopted by init\n");
                _exit(0);
            }
            else {
                sleep(3);
                printf("parent exit\n");
                _exit(0);
            }

        default:
            pid = wait(NULL);
            printf("got parent: %d\n", pid);
            _exit(0);
    }
}

int main() {
    // test26_1();
    test26_2();
    return 0;
}
