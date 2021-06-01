#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
    char buf[1];
    int p1[2]; //parent -> child
    int p2[2]; //child -> parent
    int pid;
    pipe(p1);
    pipe(p2);

    if (Fork() == 0) {  //child
        pid = getpid();
        Read(p1[0], buf, 1);
        fprintf(1, "%d: received ping\n", pid);
        Write(p2[1], buf, 1);
    } else {    //parent
        pid = getpid();
        Write(p1[1], buf, 1);
        Read(p2[0], buf, 1);
        fprintf(1, "%d: received pong\n", pid);
    }
    exit(0);
}
