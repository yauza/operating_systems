
#define CW06_COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>

typedef struct Client{
    char* name;
    char* queue;
    int id;
    int busy;
} Client;

#define MSG_SIZE 255
#define MAX_CLIENTS 10

#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5
#define MESSAGE 6

#define SERVER_NAME ("/queues-server")

// metody użytkowe
char* get_substring(char* str, int start, int len);
char* generate_random_name(int length);

// metody związane z komunikatami
int send(mqd_t desc, char* msg_pointer, int type);
int receive(mqd_t desc, char* msg_pointer, unsigned int* type_pointer);
int create_queue(char* name);
void delete_queue(char* name);
void close_queue(mqd_t descr);
int get_queue(char* name);
void get_notification(mqd_t desc, struct sigevent *s);


