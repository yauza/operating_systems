#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc < 2){
        perror("Wrong number of arguments...");
        exit(EXIT_FAILURE);
    }

    int mask_or_pending = (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) ? 1 : 0;

    if (strcmp(argv[1], "pending") != 0) {
        printf("Raising signal inside exec...\n");
        raise(SIGUSR1);
    }
    if (mask_or_pending) {
        sigset_t mask;
        sigpending(&mask);
        printf("Signal pending(exec): %s\n", sigismember(&mask, SIGUSR1) ? "true" : "false");
    }

    return 0;
}


