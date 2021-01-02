#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
    char buf[1];
    int p1[2]; //parent -> child
    int p2[2]; //child -> parent
    int pid;
    pipe(p1);
    pipe(p2);
    int n;

    if ((pid = fork()) < 0) {
        fprintf(2, "fork: fork system call error\n");
        exit(1);
    }

    if (pid == 0) {  //child
        pid = getpid();
        if((n = read(p1[0], buf, 1)) < 0) {
            fprintf(2, "read: read system call error\n");
            exit(1);
        }
        fprintf(1, "%d: received ping\n", pid);
        if((n = write(p2[1], buf, 1)) < 0) {
            fprintf(2, "write: write system call error\n");
            exit(1);
        }
    } else {    //parent
        pid = getpid();
        if((n = write(p1[1], buf, 1)) < 0) {
            fprintf(2, "write: write system call error\n");
            exit(1);
        }
        if((n = read(p2[0], buf, 1)) < 0) {
            fprintf(2, "read: read system call error\n");
            exit(1);
        }
        fprintf(1, "%d: received pong\n", pid);
    }
    exit(0);
}