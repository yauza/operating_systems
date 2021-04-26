#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int signals_to_send, received_signals;
int sig, end_sig;
int flag = 0, response = 0;
char* mode;

void sig_handler(int sig_num, siginfo_t *info, void* ucontext){
    if(strcmp(mode, "sigqueue") == 0) printf("Received signal(sender) %d...\n", info->si_value.sival_int);
    if(sig_num == end_sig){
        printf("Sent %d signals, received back %d(sender).\n", signals_to_send, received_signals);
        flag = 1;
        response = 1;
    }else{
        printf("SENDER +\n");
        response = 1;
        received_signals++;
    }
}


int main(int argc, char** argv){
    if(argc < 4){
        perror("Wrong number of arguments...(sender)");
        exit(EXIT_FAILURE);
    }

    received_signals = 0;

    int catcher_pid = atoi(argv[1]);
    signals_to_send = atoi(argv[2]);
    mode = argv[3];

    if(strcmp(mode, "kill") == 0){
        sig = SIGUSR1;
        end_sig = SIGUSR2;
    }else if(strcmp(mode, "sigrt") == 0){
        sig = SIGUSR1;
        end_sig = SIGUSR2;
    }else if(strcmp(mode, "sigqueue") == 0){
        sig = SIGRTMIN;
        end_sig = SIGRTMIN + 1;
    }


    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sig_handler;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, sig);
    sigaddset(&act.sa_mask, end_sig);

    sigaction(sig, &act, NULL);
    sigaction(end_sig, &act, NULL);

    printf("*** SENDER PID: %d ***\n", getpid());

    union sigval val = {.sival_int = 0};
    for(int i = 0; i < signals_to_send; i++){
        if(strcmp(mode, "kill") == 0 || strcmp(mode, "sigrt") == 0){
            kill(catcher_pid, sig);
        }else if(strcmp(mode, "sigqueue") == 0){
            sigqueue(catcher_pid, sig, val);
        }
        printf("SENDER ->\n");
        response = 0;
        while(!response);
    }

    if(strcmp(mode, "kill") == 0 || strcmp(mode, "sigrt") == 0){
        kill(catcher_pid, end_sig);
    }else if(strcmp(mode, "sigqueue") == 0){
        sigqueue(catcher_pid, end_sig, val);
    }

    response = 0;
    while(!response);

    while(!flag);

    return 0;
}




