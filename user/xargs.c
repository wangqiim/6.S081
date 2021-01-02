#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char** argv) {
    char buf[256];
    char *new_argvs[MAXARG];
    char *p;
    if (argc < 2) {
        fprintf(2, "xargs: xargs [-n x] argv...\n");
        exit(1);
    }
    read(0, buf, 256);//strchr
    p = buf;

    for (int i = 1; i < argc; i++) {
        new_argvs[i - 1] = argv[i];
        //printf("argc = %d, argvs[%d] = %s\n", argc, i - 3, new_argvs[i - 3]);
    }
    while (1) {
        new_argvs[argc - 1] = p;
        new_argvs[argc] = 0;
        p = strchr(p, '\n');
        if (p == 0) break;
        *(p++) = 0;
        if (fork() == 0) {  //child
            exec(new_argvs[0], new_argvs);
        } else {
            wait(0);
        }
    }
    exit(0);
}