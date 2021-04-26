#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sig_handler(int sig_num){
    printf("Signal received: %d\n", sig_num);
}

int main(int argc, char** argv){
	if(argc < 3){
	    perror("Wrong number of arguments...");
	    exit(EXIT_FAILURE);
	}

	int mask_or_pending = (strcmp(argv[2], "mask") == 0 || strcmp(argv[2], "pending") == 0) ? 1 : 0;

	if (strcmp(argv[2], "ignore") == 0) {
	    signal(SIGUSR1, SIG_IGN);
	} else if (strcmp(argv[2], "handler") == 0) {
	    signal(SIGUSR1, sig_handler);
	} else if (mask_or_pending) {
        sigset_t mask;
	    sigemptyset(&mask);
	    sigaddset(&mask, SIGUSR1);
	    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0){
	        perror("Cannot block the signal...");
	    }
	}

	printf("Raising signal inside parent...\n");
	raise(SIGUSR1);
    sigset_t mask;

	if(mask_or_pending){
        sigpending(&mask);
        printf("Signal pending(parent): %s\n", sigismember(&mask, SIGUSR1) ? "true" : "false");
	}

	if(strcmp(argv[1], "fork") == 0){
	    pid_t pid = fork();
	    if(pid == 0){
            if (strcmp(argv[2], "pending") != 0){
                printf("Raising signal inside child...\n");
                raise(SIGUSR1);
            }
            if (mask_or_pending){
                sigpending(&mask);
                printf("Signal pending(child): %s\n", sigismember(&mask, SIGUSR1) ? "true" : "false");
            }
	    }
	}else if(strcmp(argv[1], "exec") == 0){
	    execl("./exec", "./exec", argv[2], NULL);
	}
	return 0;
}
