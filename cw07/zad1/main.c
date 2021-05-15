#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include "common.h"

pid_t processes[N + M];
int semaphore_id, memory_id;

void clear()
{
    semctl(semaphore_id, 0, IPC_RMID, NULL);
    shmctl(memory_id, IPC_RMID, NULL);
    system("make clean");
}

void sig_handler(int sig_num){
    for(int i = 0; i < N + M; i++) kill(processes[i], SIGINT);
    clear();
    exit(0);
}

void init_pizzeria(){
    pid_t child;
    for(int i = 0; i < N; i++){
        child = fork();
        if(child == 0){
            execlp("./cook", "cook", NULL);
        }
        processes[i] = child;
    }

    for(int i = 0; i < M; i++){
        child = fork();
        if(child == 0){
            execlp("./delivery", "delivery", NULL);
        }
        processes[N + i] = child;
    }

    for(int i = 0; i < N + M; i++) wait(NULL);
}

int main(int argc, char** argv){
    signal(SIGINT, sig_handler);

    key_t semaphore_key = ftok("main", 1);
    semaphore_id = semget(semaphore_key, 6, IPC_CREAT | 0666);
    // licznik pizz w piecu
    semctl(semaphore_id, 0, SETVAL, 0);
    semctl(semaphore_id, 1, SETVAL, MAX_PIZZA);
    // licznik pizz na stole
    semctl(semaphore_id, 2, SETVAL, 0);
    semctl(semaphore_id, 3, SETVAL, MAX_PIZZA);
    semctl(semaphore_id, 4, SETVAL, 0);
    semctl(semaphore_id, 5, SETVAL, 0);

    key_t memory_key = ftok("main", 1);
    memory_id = shmget(memory_key, sizeof(pizzas), IPC_CREAT | 0666);
    struct pizzas *p = shmat(memory_id, NULL, 0);
    p->add_index = 0;
    p->get_index = 0;
    init_pizzeria();
    clear();
    return 0;

}
