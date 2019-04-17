#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "fake-pi.h"
#include <assert.h>

int exit_code(int pid) {
    int status;
    if(waitpid(pid, &status, 0) < 0)
        sys_die("waitpid", waitpid failed?);
    if(!WIFEXITED(status))
        panic("%d: unexpected crash\n", pid);
    return WEXITSTATUS(status);
}

void fancy_check(int N) {
    trace("running %d trials\n", N);
    for(int i = 0; i < N; i++) {
        trace("running iteration %d\n", i);
        fake_gpio_set_bias(i);

        int pid = fork();
        if(pid < 0)
            sys_die(fork, cannot fork process!);

        // child
        if(!pid) {
            srandom(i);
            notmain();
            fflush(stdout);
            exit(0);
        } else {
            int ret = exit_code(pid);
            if(ret != 0)
                panic("unexpected exit value=%d\n", ret);
            fflush(stdout);
        }
    }
    trace("DONE!\n");
}

int main(int argc, char *argv[]) {
    if(argc > 2) {
        fprintf(stderr, "usage error: %s [0..3]\n", argv[0]);
        exit(1);
    }

    int n = 0;
    if(argv[1])
        n = atoi(argv[1]);

    fake_timer_max_delta(55000);

    srandom(49);
    // make sure that everyone has the same random.
    assert(89789692 == random());
    if(n)
        fancy_check(n);
    else
        notmain();
    return 0;
}
