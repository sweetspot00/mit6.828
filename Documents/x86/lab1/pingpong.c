#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int parent_fd[2];
    int child_fd[2];
    int buff[4];
    int pid;

    pipe(parent_fd);
    pipe(child_fd);
    if (fork() == 0) {
        //child
        pid = getpid();
        read(parent_fd[0], buff, 4);//1 byte
        printf("%d: received %s\n", pid, buff);
        write(child_fd[1], "pong", 4);
    } else {
        pid = getpid();
        write(parent_fd[1], "ping", 4);
        read(child_fd[0], buff, 4);
        printf("%d: received %s\n", pid, buff);
    }
    exit(0);
}