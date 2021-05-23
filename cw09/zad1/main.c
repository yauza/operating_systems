#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define REINDEERS 9
#define MAX_GIFTS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_santa = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_reindeers = PTHREAD_COND_INITIALIZER;

int elves_with_problems;
int waiting_reindeers;

void* reindeer(void* arg){
    int* r = arg;
    int id = *r;

    while(1){
        // Jeżeli wszystkie renifery wróciły
        pthread_mutex_lock(&mutex);
        while(waiting_reindeers > 0) pthread_cond_wait(&cond_reindeers, &mutex);
        pthread_mutex_unlock(&mutex);

        printf("[Renifer %d] dostarcza prezenty.\n", id);
        sleep(rand()%3 + 2);


        printf("[Renifer %d] jest na wakacjach.\n", id);
        sleep(rand()%6 + 5);


        pthread_mutex_lock(&mutex);

        waiting_reindeers++;
        printf("[Renifer %d] czeka %d reniferów na Mikołaja.\n", id, waiting_reindeers);

        if(waiting_reindeers == REINDEERS) pthread_cond_signal(&cond_santa);

        pthread_mutex_unlock(&mutex);
    }
}

void* santa(void* arg){
    int gifts = 0;
    while(gifts < MAX_GIFTS){
        pthread_mutex_lock(&mutex);
        while(waiting_reindeers != REINDEERS) pthread_cond_wait(&cond_santa, &mutex);

        printf("[MIKOŁAJ] Budzę się.\n");

        printf("[MIKOŁAJ] Dostarczam zabawki.\n");
        sleep(rand()%3 + 2);
        gifts++;
        waiting_reindeers = 0;

        // odblokowujemy renifery
        pthread_cond_broadcast(&cond_reindeers);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char** argv){
	pthread_t* threads = calloc(1 + REINDEERS, sizeof(pthread_t));
	waiting_reindeers = 0;

	// SANTA
	pthread_create(&threads[0], NULL, santa, (void*)0);

	int* args = calloc(REINDEERS, sizeof(int));
	// REINDEERS
	for(int i = 1; i < REINDEERS + 1; i++){
	    args[i - 1] = i;
        pthread_create(&threads[i], NULL, reindeer, (void*)&args[i - 1]);
	}

	pthread_join(threads[0], NULL);

	for(int i = 0; i < 1 + REINDEERS; i++) pthread_cancel(threads[i]);

	free(threads);
	return 0;
}
