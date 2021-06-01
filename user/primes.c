#include "kernel/types.h"
#include "user/user.h"

void primes(int left_pipe_in, int num) {
    if (num > 11)
        exit(0);
    int right_pipe[2];
    int pid;
    pipe(right_pipe);

    if ((pid = Fork()) == 0) { // child
        Close(right_pipe[1]);
        primes(right_pipe[0], num + 1);
    } else {    //parent
        Close(right_pipe[0]);
        int p, n;
        Read(left_pipe_in, &p, sizeof(p));
        printf("prime %d\n", p);
        //read returns zero when the write-side of a pipe is closed.
        for (int i = 1; Read(left_pipe_in, &n, sizeof(n)) != 0; i++) {
            if (n % p != 0)
                Write(right_pipe[1], &n, sizeof(n));
        }
        Close(right_pipe[1]);
        wait(0);
    }
}

int main(int argc, char**argv) {
    int left_pipe[2];
    int pid;
    pipe(left_pipe);

    if((pid = Fork()) == 0) { //child
        Close(left_pipe[1]);
        primes(left_pipe[0], 1);
    } else {
        Close(left_pipe[0]);
        for (int i = 2; i <= 35; i++) {
            Write(left_pipe[1], &i, sizeof(i));
        }
        Close(left_pipe[1]);
        wait(0);
    }
    exit(0);
}
