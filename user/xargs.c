#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void read_command();
int split_lines(int);
void run_command(int);

char buffer[1024];
char command_buf[MAXARG][128];

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage: xargs <command>\n");
        exit();
    } 

    for (int i = 0; i < argc - 1; i++) {
        strcpy(command_buf[i], argv[i + 1]);
    }
    int num_args = 0;
    while(1) {
        read_command();
        num_args = split_lines(argc - 1);
        run_command(num_args);
    }
}

void read_command() {
    int i = 0;
    char c = 0;
    while(read(0, &c, 1) == 1) {
        if (c == '\n') {
            buffer[i] = 0;
            return;
        }
        buffer[i++] = c;
    }
    if (i == 0) exit();
}

int split_lines(int start) {
    int i = 0, j = 0;
    int command_pos = start;
    if (!buffer[0]) exit();
    while(buffer[i]) {
        if (buffer[i] == ' ' && j != 0) {
            command_buf[command_pos][j] = 0; // null terminate the string
            j = 0;
            command_pos += 1;
        } else {
            command_buf[command_pos][j++] = buffer[i++];
        }
        
    }
    command_buf[command_pos][j] = 0;
    return command_pos + 1;
}

void run_command(int num_args) {
    char *command[num_args + 1];
    for (int i = 0; i < num_args; i++) {
        command[i] = command_buf[i];
    }
    command[num_args] = 0;
    if(fork() == 0) {
        exec(command[0], command);
    } else {
        wait();
    }
}