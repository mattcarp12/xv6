#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv) {
    if (argc != 2) {
        printf("Usage: sleep <amount of time to sleep>\n");
        exit();
    } 
    
}