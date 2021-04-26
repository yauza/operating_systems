#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/resource.h>
#include <unistd.h>
#include "common.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

// lista klientów: klient + klucz jego kolejki
int clients[MAX_CLIENTS][2];
int connections[MAX_CLIENTS];
int client_counter = 0;

key_t public_key;
int queue;
void sigint_handler() { exit(0); }
void fill_clients(){ for(int i = 0; i < MAX_CLIENTS; i++) {clients[i][0] = -1; clients[i][1] = -1;} }
void fill_connections(){ for(int i = 0; i < MAX_CLIENTS; i++) connections[i] = -1; }

void clean_on_exit(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][0] != -1) {
            struct Message msg;
            msg.mtype = 1;

            if (send(get_queue(clients[i][1]), &msg) < 0) {
                perror("msgsnd clean_on_exit");
                exit(EXIT_FAILURE);
            }
        }
    }
    delete_queue(queue);
}


int get_client_id(int client_key){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][1] == client_key) return i;
    }
    return -1;
}

void init_client(struct Message msg){
    key_t key = msg.client_key;
    if(client_counter >= MAX_CLIENTS){
        printf("Cannot add another client...\n");
        return;
    }
    int i = 0;
    while(i < MAX_CLIENTS){
        if(clients[i][0] == -1) break;
        i++;
    }
    clients[i][0] = 0;
    clients[i][1] = key;
    client_counter++;

    struct Message confirmation;
    sprintf(confirmation.message_text, "%s", "[SERVER CONFIRMATION]\n");
    confirmation.mtype = MESSAGE;
    confirmation.order = MESSAGE;
    int client_queue = get_queue(msg.client_key);
    printf("Sending confirmation - key: %d, queue_id: %d\n", msg.client_key, client_queue);
    if(send(client_queue, &confirmation) < 0){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

void stop_client(struct Message msg){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][1] == msg.client_key){
            clients[i][0] = -1;
            clients[i][1] = -1;
            break;
        }
    }
}

void list_clients(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][0] != -1){
            char status[20];
            if(clients[i][0] == 0){
                sprintf(status, "%s", "FREE");
            }else{
                sprintf(status, "%s", "BUSY");
            }
            printf("Client nr: %d, key: %d, status: %s\n", i, clients[i][1], status);
        }
    }
}

void connect_to_client(struct Message msg){
    int client_id = atoi(msg.message_text);
    int client_key = clients[client_id][1];

    struct Message message1;
    message1.mtype = 4;
    sprintf(message1.message_text, "%d", client_key);
    if(send(get_queue(msg.client_key), &message1) < 0){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    clients[get_client_id(msg.client_key)][0] = 1;

    connections[get_client_id(msg.client_key)] = client_id;
    connections[client_id] = get_client_id(msg.client_key);

    struct Message message2;
    message2.mtype = 4;
    sprintf(message2.message_text, "%d", msg.client_key);
    if(send(get_queue(client_key), &message2) < 0){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    clients[client_id][0] = 1;
    printf(" Sent information to clients to connect. \n");
}

void disconnect(struct Message msg){
    int client1 = msg.client_key;
    int client2 = clients[connections[get_client_id(msg.client_key)]][1];

    printf("client1: %d, client2: %d -> ", get_client_id(client1), get_client_id(client2));

    clients[get_client_id(client1)][0] = 0;
    clients[get_client_id(client2)][0] = 0;

    struct Message message;
    message.mtype = 2;
    if(send(get_queue(client1), &message) < 0){
        perror("msgsnd disconnect");
        exit(EXIT_FAILURE);
    }

    if(send(get_queue(client2), &message) < 0){
        perror("msgsnd disconnect");
        exit(EXIT_FAILURE);
    }
    printf("Sent information to clients.\n");
}

void handle_message(struct Message msg){
    switch(msg.mtype){
        // 1
        case STOP:{
            printf("[STOP]\n");
            stop_client(msg);
            break;
        }
        // 2
        case DISCONNECT:{
            printf("[DISCONNECT]");
            disconnect(msg);
            break;
        }
        // 3
        case LIST:{
            printf("[LIST]\n");
            list_clients();
            break;
        }
        // 4
        case CONNECT:{
            printf("[CONNECT]");
            connect_to_client(msg);
            break;
        }
        // 5
        case INIT:{
            printf("[INIT]");
            init_client(msg);
            break;
        }
        // 6
        case MESSAGE:{
            printf("[MESSAGE] - ");
            printf("%s", msg.message_text);
            break;
        }
        default:{
            printf("Message without specified type...\n");
            break;
        }

    }
}



int main(int argc, char** argv){
    fill_clients();
    atexit(clean_on_exit);
    public_key = ftok(getenv("HOME"), 1);
    signal(SIGINT, sigint_handler);
    if((queue = msgget(public_key, IPC_CREAT | 0666)) < 0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] key: %d, queue_id: %d\n", public_key, queue);

    struct Message buffer;

    while(1){
        while(queue_size(queue) > 0) {
            if (receive(queue, &buffer) < 0) {
                perror("[SERVER] message receive failed...\n");
                exit(EXIT_FAILURE);
            }

            handle_message(buffer);
        }
    }

}