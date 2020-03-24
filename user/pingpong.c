#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int parent_fd[2], child_fd[2];
    char byte = 't';
    pipe(parent_fd);
    pipe(child_fd);
    write(parent_fd[1], &byte, 1);
    if (fork() == 0) {
        int child_pid = getpid();
        read(parent_fd[0], &byte, 1);
        printf("%d: received ping\n", child_pid);
        write(child_fd[1], &byte, 1);
        exit();
    } else {
        wait();
        int parent_pid = getpid();
        read(child_fd[0], &byte, 1);
        printf("%d: received pong\n", parent_pid);
    }
    exit();
}