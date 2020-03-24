#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 35

void sieve(int *fd) {
    if (fork() == 0) {
        
        /* Create pipe to send data to child process */
        int child_fd[2]; pipe(child_fd);

        /* Close write end of parent pipe, since will
            only write to child, not parent */
        close(fd[1]);


        int first, num;
        /* Read first number from parent pipe, will always 
            be a prime */
        if (read(fd[0], &first, sizeof(first)) > 0) {
            printf("prime %d\n", first);

            /* Read from parent pipe, write to child
                if not divisible by first prime (sieve) */
            while (read(fd[0], &num, sizeof(num)) > 0) {
                if (num % first != 0) {
                    write(child_fd[1], &num, sizeof(num));
                }
            }

            /* Start new child process for next stage of pipeline */
            sieve(child_fd);
        }

        /* Close read end of pipe */
        close(fd[0]);

        /* Wait for and child processes to complete */
        wait();

        /* Exit child process */
        exit();
    }
    close(fd[0]); close(fd[1]);
    wait();
}

int main() {
    /* Create first pipe */
    int fd[2]; pipe(fd);

    /* Seed the pipeline */
    for (int i = 2; i <= MAX; i++)
        write(fd[1], &i, sizeof(i));
    
    /* call next pipeline */
    sieve(fd);

    exit();
}