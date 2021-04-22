#include "../apue.h"

int main() {
    int fd = open("/tmp/data.txt", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
    if(fd == -1)
        return -1;
    int times = 10;
    char *buf = "daemon\n";
    while(times--) {
        write(fd, buf, 7);
    }
    close(fd);
    return 0;
}
