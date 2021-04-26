#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void status_handler(int sig_num, siginfo_t *info, void* ucontext){
    printf("\n[STATUS HANDLER]\n");
    printf("Number: %d\n", info->si_signo);
    printf("PID: %d\n", info->si_pid);
    printf("Status: %d\n", info->si_status);
    printf("Code: %d\n", info->si_code);
    printf("Errno: %d\n", info->si_errno);
}

void value_handler(int sig_num, siginfo_t *info, void* ucontext){
    printf("\n[VALUE HANDLER]\n");
    printf("Number: %d\n", info->si_signo);
    printf("PID: %d\n", info->si_pid);
    printf("Code: %d\n", info->si_code);
    printf("Value: %d\n", info->si_value.sival_int);
    printf("Errno: %d\n", info->si_errno);
}

void sigint_handler(int sig_num, siginfo_t *info, void* ucontext){
    printf("\n[SIGINT HANDLER]\n");
    printf("Number: %d\n", info->si_signo);
    printf("PID: %d\n", info->si_pid);
    printf("Status: %d\n", info->si_status);
    printf("Code: %d\n", info->si_code);
    printf("Errno: %d\n", info->si_errno);
}

void test_handler(int sig_num, siginfo_t *info, void* ucontext){
    raise(SIGUSR2);
    printf("***************************");
}


int main(int argc, char** argv){

    // 1
    // przechwytywanie sygnału wysyłanego przez potomka kończącego działanie
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = status_handler;
    sigaction(SIGCHLD, &act, NULL);
    pid_t pid = fork();
    if(pid == 0) exit(88);
    wait(NULL);


    // 2
    // przekazywanie wartości val w sygnale
    struct sigaction act2;
    act2.sa_flags = SA_SIGINFO;
    act2.sa_sigaction = value_handler;
    sigaction(SIGUSR1, &act2, NULL);
    union sigval val = {.sival_int = 2137};
    sigqueue(getpid(), SIGUSR1, val);


    // 3
    // brak informacji o zatrzymanym procesie potomnym
    struct sigaction act3;
    act3.sa_flags = SA_NOCLDSTOP;
    act3.sa_sigaction = status_handler;
    sigaction(SIGCHLD, &act3, NULL);
    pid = fork();
    if(pid == 0) exit(88);
    wait(NULL);


    // 4
    // bez flagi wywołuje się w nieskończoność
    struct sigaction act4;
    act4.sa_flags = SA_RESETHAND;
    act4.sa_sigaction = test_handler;
    sigaction(SIGUSR2, &act4, NULL);
    sigqueue(getpid(), SIGUSR2, val);


    // 5
    // podczas zatrzymywania procesu ctrl+c
    struct sigaction act5;
    act5.sa_flags = SA_SIGINFO;
    act5.sa_sigaction = sigint_handler;
    sigaction(SIGINT, &act5, NULL);
    sleep(10);

    return 0;
}


