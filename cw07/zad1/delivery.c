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

int semaphore_id, memory_id;
int pizza_type, number_of_pizzas;
typedef struct sembuf sembuf;

void get_pizza(){

    if(semctl(semaphore_id, 3, GETVAL, NULL) == 5) semctl(semaphore_id, 5, SETVAL, 1);

    sembuf *check = calloc(4, sizeof(sembuf));
    check[0].sem_num = 5;
    check[0].sem_op = 0;
    check[0].sem_flg = 0;

    check[1].sem_num = 5;
    check[1].sem_op = 1;
    check[1].sem_flg = 0;

    check[2].sem_num = 4;
    check[2].sem_op = 0;
    check[2].sem_flg = 0;

    check[3].sem_num = 4;
    check[3].sem_op = 1;
    check[3].sem_flg = 0;
    semop(semaphore_id, check, 4);

    struct pizzas *p = shmat(memory_id, NULL, 0);
    pizza_type = p->values[p->get_index];
    p->get_index++;
    if(p->get_index == MAX_PIZZA) p->get_index = 0;

    sembuf *take = calloc(2, sizeof(sembuf));
    take[0].sem_num = 3;
    take[0].sem_op = 1;
    take[0].sem_flg = 0;

    take[1].sem_num = 4;
    take[1].sem_op = -1;
    take[1].sem_flg = 0;
    semop(semaphore_id, take, 2);

    number_of_pizzas = MAX_PIZZA - semctl(semaphore_id, 3, GETVAL, NULL);

    printf("(%d %ld) Pobieram pizze: %d. Liczba pizz na stole: %d\n", getpid(), time(NULL), pizza_type, number_of_pizzas);

    if(semctl(semaphore_id, 2, GETVAL, NULL) == 1) semctl(semaphore_id, 2, SETVAL, 0);
}

void deliver_pizza(){
    printf("(%d %ld) Dostarczam pizze: %d\n", getpid(), time(NULL), pizza_type);
}

int main(int argc, char** argv){
    srand(getpid());

    key_t key = ftok("main", 1);
    semaphore_id = semget(key, 6, 0);
    memory_id = shmget(key, sizeof(pizzas), 0);

    while(1){
        if(semctl(semaphore_id, 3, GETVAL, NULL) < 5){
            get_pizza();
            sleep(rand() % 6);
            deliver_pizza();
            sleep(rand() % 6);
        }
    }
}

