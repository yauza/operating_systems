#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
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

void put_in_oven(){
    sembuf *load = calloc(3, sizeof(sembuf));
    // najpierw czekamy na zwolnienie semafora
    load[0].sem_num = 0;
    load[0].sem_op = 0;
    load[0].sem_flg = 0;
    // blokujemy semafor
    load[1].sem_num = 0;
    load[1].sem_op = 1;
    load[1].sem_flg = 0;

    load[2].sem_num = 1;
    load[2].sem_op = -1;
    load[2].sem_flg = 0;

    semop(semaphore_id, load, 3);

    number_of_pizzas = MAX_PIZZA - semctl(semaphore_id, 1, GETVAL, NULL);

    sembuf *unblock = calloc(1, sizeof(sembuf));
    // odblokowujemy semafor
    unblock[0].sem_num = 0;
    unblock[0].sem_op = -1;
    unblock[0].sem_flg = 0;
    semop(semaphore_id, unblock, 1);

    printf("(%d %ld) Dodałem pizze: %d. Liczba pizz w piecu: %d\n", getpid(), time(NULL), pizza_type,
           number_of_pizzas);
}

void remove_from_oven(){
    sembuf *load2 = calloc(3, sizeof(sembuf));
    // najpierw czekamy na zwolnienie semafora
    load2[0].sem_num = 0;
    load2[0].sem_op = 0;
    load2[0].sem_flg = 0;
    // blokujemy semafor
    load2[1].sem_num = 0;
    load2[1].sem_op = 1;
    load2[1].sem_flg = 0;

    load2[2].sem_num = 1;
    load2[2].sem_op = 1;
    load2[2].sem_flg = 0;
    semop(semaphore_id, load2, 3);

    number_of_pizzas = MAX_PIZZA - semctl(semaphore_id, 1, GETVAL, NULL);

    sembuf *unblock2 = calloc(1, sizeof(sembuf));
    unblock2[0].sem_num = 0;
    unblock2[0].sem_op = -1;
    unblock2[0].sem_flg = 0;
    semop(semaphore_id, unblock2, 1);

    // --------------------------------------------------
    // obsługa stołu

    sembuf *t = calloc(6, sizeof(sembuf));
    t[0].sem_num = 2;
    t[0].sem_op = 0;
    t[0].sem_flg = 0;

    t[1].sem_num = 2;
    t[1].sem_op = 1;
    t[1].sem_flg = 0;

    t[2].sem_num = 4;
    t[2].sem_op = 0;
    t[2].sem_flg = 0;

    t[3].sem_num = 4;
    t[3].sem_op = 1;
    t[3].sem_flg = 0;

    t[4].sem_num = 3;
    t[4].sem_op = -1;
    t[4].sem_flg = 0;

    t[5].sem_num = 4;
    t[5].sem_op = -1;
    t[5].sem_flg = 0;

    semop(semaphore_id, t, 6);

    struct pizzas *p = shmat(memory_id, NULL, 0);
    p->values[p->add_index] = pizza_type;
    p->add_index++;
    if(p->add_index == MAX_PIZZA) p->add_index = 0;

    int number_of_pizzas_table = MAX_PIZZA - semctl(semaphore_id, 3, GETVAL, NULL);
    printf("(%d %ld) Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d\n", getpid(), time(NULL), pizza_type,
           number_of_pizzas, number_of_pizzas_table);

    if(semctl(semaphore_id, 3, GETVAL, NULL) < MAX_PIZZA) semctl(semaphore_id, 5, SETVAL, 0);
    if(semctl(semaphore_id, 3, GETVAL, NULL) > 0) semctl(semaphore_id, 2, SETVAL, 0);
}


int main(int argc, char** argv){
    srand(getpid());

    key_t key = ftok("main", 1);
    semaphore_id = semget(key, 6, 0);
    memory_id = shmget(key, sizeof(pizzas), 0);

    while(1){
        if(semctl(semaphore_id, 1, GETVAL, NULL) > 0) {
            pizza_type = rand() % 10;
            printf("(%d %ld) Przygotowuję pizze: %d\n", getpid(), time(NULL), pizza_type);
            sleep(rand() % 3);

            put_in_oven();

            sleep(rand() % 4);

            remove_from_oven();
        }
    }
}

