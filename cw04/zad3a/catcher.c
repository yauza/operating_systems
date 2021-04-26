#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int received_signals = 0, flag = 0;
int sig, end_sig;
pid_t sender_pid;
char* mode;

void sig_handler(int sig_num, siginfo_t *info, void* ucontext){
    //if(strcmp(mode, "sigqueue") == 0) printf("Received signal %d...\n", info->si_value.sival_int);
    if(sig_num == end_sig){
        printf("Received %d signals(catcher).\n", received_signals);
        sender_pid = info->si_pid;
        flag = 1;
    }else{
        if(flag == 0) received_signals++;
        //response = 1;
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        perror("Wrong number of arguments...");
        exit(EXIT_FAILURE);
    }
    mode = argv[1];

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
    sigaction(sig, &act, NULL);
    sigaction(end_sig, &act, NULL);

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, sig);
    sigaddset(&act.sa_mask, end_sig);

    printf("*** CATCHER PID: %d ***\n", getpid());

    while(!flag);

    union sigval val;
    for(int i = 0; i < received_signals; i++){
        if(strcmp(mode, "kill") == 0 || strcmp(mode, "sigrt") == 0){
            kill(sender_pid, sig);
        }else if(strcmp(mode, "sigqueue") == 0){
            val.sival_int = i;
            sigqueue(sender_pid, sig, val);
        }

    }
    val.sival_int = (int)received_signals;
    if(strcmp(mode, "kill") == 0 || strcmp(mode, "sigrt") == 0){
        kill(sender_pid, end_sig);
    }else if(strcmp(mode, "sigqueue") == 0){
        sigqueue(sender_pid, end_sig, val);
    }

    return 0;
}




