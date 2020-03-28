#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 35

void sieve(int read_fd) {
    int pid = fork();
    if (pid == 0) {

        /* Create pipe to send data to child process */
        int child_fd[2]; pipe(child_fd);

        int first, num;
        /* Read first number from parent pipe, will always 
            be a prime */
        if (read(read_fd, &first, sizeof(first)) > 0) {
            printf("prime %d\n", first);

            /* Read from parent pipe, write to child
                if not divisible by first prime (sieve) */
            while (read(read_fd, &num, sizeof(num)) > 0) {
                if (num % first != 0) {
                    write(child_fd[1], &num, sizeof(num));
                }
            }
            close(child_fd[1]);
            /* Start new child process for next stage of pipeline */
            sieve(child_fd[0]);
        }
        /* Exit child process */
        exit();
    } else if (pid > 0) {
        close(read_fd);
        wait();
    } else printf("Fork error.\n");
}

int main() {
    /* Create first pipe */
    int fd[2]; pipe(fd);

    /* Seed the pipeline */
    for (int i = 2; i <= MAX; i++)
        write(fd[1], &i, sizeof(i));
    close(fd[1]);
    
    /* call next pipeline */
    sieve(fd[0]);

    exit();
}