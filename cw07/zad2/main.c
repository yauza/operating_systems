#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include "common.h"

pid_t processes[N + M];

void sig_handler(int sig_num){
    for(int i = 0; i < N + M; i++) kill(processes[i], SIGINT);
    system("make clean");
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

    sem_t *s1 = sem_open("/s1", O_CREAT | O_RDWR, 0666, 0);
    sem_t *s2 = sem_open("/s2", O_CREAT | O_RDWR, 0666, MAX_PIZZA);
    sem_t *s3 = sem_open("/s3", O_CREAT | O_RDWR, 0666, 0);
    sem_t *s4 = sem_open("/s4", O_CREAT | O_RDWR, 0666, MAX_PIZZA);
    sem_t *s5 = sem_open("/s5", O_CREAT | O_RDWR, 0666, 0);
    sem_t *s6 = sem_open("/s6", O_CREAT | O_RDWR, 0666, 0);

    int fd = shm_open("shared_memory", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    ftruncate(fd, sizeof(pizzas));
    struct pizzas *p = mmap(NULL, sizeof(pizzas), PROT_WRITE, MAP_SHARED, fd, 0);
    p->add_index = 0;
    p->get_index = 0;
    munmap(p, sizeof(pizzas));

    init_pizzeria();

    sem_close(s1);
    sem_close(s2);
    sem_close(s3);
    sem_close(s4);
    sem_close(s5);
    sem_close(s6);

    sem_unlink("/s1");
    sem_unlink("/s2");
    sem_unlink("/s3");
    sem_unlink("/s4");
    sem_unlink("/s5");
    sem_unlink("/s6");

    shm_unlink("/shared_memory");

    return 0;

}
