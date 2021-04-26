
#ifndef CW06_COMMON_H
#define CW06_COMMON_H

typedef struct Message{
    long mtype;
    int order;
    int client_key;
    char message_text[4096];
} Message;

#define MSG_SIZE sizeof(struct Message) - sizeof(long)
#define MAX_CLIENTS 10

#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5
#define MESSAGE 6

// metody użytkowe
char* get_substring(char* str, int start, int len);
void read_cmd(char* cmd, struct Message* msg);

// metody związane z komunikatami
int send(int queue_id, struct Message* msg);
int receive(int queue_id, struct Message* msg);
int create_queue(int key);
int delete_queue(int queue_id);
int get_queue(int key);
int queue_size(int queue_id);

#endif
