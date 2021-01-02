#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(2, "usage: sleep n ticks...\n");
        exit(1);
    }
    //sleep n ticks
    int n = atoi(argv[1]);
    if (sleep(n) != 0) {
        fprintf(2, "sleep: sleep system call error\n");
        exit(1);
    }
    fprintf(1, "(nothing happens for a little while)\n");
    exit(0);
}