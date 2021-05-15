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
typedef struct sembuf sembuf;

void get_pizza(){
//    int index = (MAX_PIZZA - semctl(semaphore_id, 1, GETVAL, NULL)) - 1;
//    struct pizzas *p = shmat(memory_id, NULL, 0);
//    pizza_type = p->values[index];
    pizza_type = -1;
    if(semctl(semaphore_id, 3, GETVAL, NULL) == 5) semctl(semaphore_id, 5, SETVAL, 1);

    sembuf *take = calloc(6, sizeof(sembuf));
    take[0].sem_num = 5;
    take[0].sem_op = 0;
    take[0].sem_flg = 0;

    take[1].sem_num = 5;
    take[1].sem_op = 1;
    take[1].sem_flg = 0;

    take[2].sem_num = 4;
    take[2].sem_op = 0;
    take[2].sem_flg = 0;

    take[3].sem_num = 4;
    take[3].sem_op = 1;
    take[3].sem_flg = 0;

    take[4].sem_num = 3;
    take[4].sem_op = 1;
    take[4].sem_flg = 0;

    take[5].sem_num = 4;
    take[5].sem_op = -1;
    take[5].sem_flg = 0;

    semop(semaphore_id, take, 6);

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

//    while(1){
//        if(semctl(semaphore_id, 3, GETVAL, NULL) < 5){
//            get_pizza();
//            sleep(rand() % 6);
//            deliver_pizza();
//            sleep(rand() % 6);
//        }
//    }
}

