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

int semaphore_id, memory_id;
int pizza_type, number_of_pizzas;

sem_t *s1;
sem_t *s2;
sem_t *s3;
sem_t *s4;
sem_t *s5;
sem_t *s6;

int fd;

void sig_handler(){
    sem_close(s1);
    sem_close(s2);
    sem_close(s3);
    sem_close(s4);
    sem_close(s5);
    sem_close(s6);

    shm_unlink("/shared_memory");
    exit(0);
}

void put_in_oven(){
    sem_post(s1);
    sem_wait(s2);

    int s2_val;
    sem_getvalue(s2, &s2_val);
    number_of_pizzas = MAX_PIZZA - s2_val;

    sem_wait(s1);

    printf("(%d %ld) Dodałem pizze: %d. Liczba pizz w piecu: %d\n", getpid(), time(NULL), pizza_type,
           number_of_pizzas);
}

void remove_from_oven(){
    sem_post(s1);
    sem_wait(s2);

    int s2_val;
    sem_getvalue(s2, &s2_val);
    number_of_pizzas = MAX_PIZZA - s2_val;

    sem_wait(s1);

    // --------------------------------------------------
    // obsługa stołu
    //printf("(%d) obsluga stołu\n", getpid());



//    int index = (MAX_PIZZA - semctl(semaphore_id, 1, GETVAL, NULL)) - 1;
//    struct pizzas *p = shmat(memory_id, NULL, 0);
//    p->values[index] = pizza_type;

    int s3_val;
    sem_getvalue(s2, &s3_val);
    int number_of_pizzas_table = MAX_PIZZA - s3_val;
    printf("(%d %ld) Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d\n", getpid(), time(NULL), pizza_type,
           number_of_pizzas, number_of_pizzas_table);

}


int main(int argc, char** argv){
    srand(getpid());
    signal(SIGINT, sig_handler);

    s1 = sem_open("/s1", O_RDWR, 0666);
    s2 = sem_open("/s2", O_RDWR, 0666);
    s3 = sem_open("/s3", O_RDWR, 0666);
    s4 = sem_open("/s4", O_RDWR, 0666);
    s5 = sem_open("/s5", O_RDWR, 0666);
    s6 = sem_open("/s6", O_RDWR, 0666);

    fd = shm_open("/shared_memory", O_RDWR, 0666);

    while(1){
        int s2_val;
        sem_getvalue(s2, &s2_val);
        if(s2_val > 0) {
            pizza_type = rand() % 10;
            printf("(%d %ld) Przygotowuję pizze: %d\n", getpid(), time(NULL), pizza_type);
            sleep(rand() % 3);

            put_in_oven();

            sleep(rand() % 4);

            remove_from_oven();
        }
    }
}

