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

#ifndef CW07_COMMON_H
#define CW07_COMMON_H

#define MAX_PIZZA 5
#define N 3
#define M 3

struct pizzas{
    int add_index;
    int get_index;
    int values[MAX_PIZZA];
} pizzas;


#endif //CW07_COMMON_H
