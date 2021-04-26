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

// lista klientów - id + deskryptor kolejki
int clients[MAX_CLIENTS][2];
char* clients_name[MAX_CLIENTS];
int connections[MAX_CLIENTS];
int client_counter = 0;

int server_queue;


void fill_clients(){ for(int i = 0; i < MAX_CLIENTS; i++) {clients[i][0] = -1; clients[i][1] = -1;} }
void fill_connections(){ for(int i = 0; i < MAX_CLIENTS; i++) connections[i] = -1; }

// zamyka i usuwa kolejke serwera
void server_exit(){
    close_queue(server_queue);
    delete_queue(SERVER_NAME);
    exit(0);
}

// wysyła polecenie zakończenia do wszystkich klientow poczym zam sie zamyka
void stop(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][0] != -1){
            char msg[MSG_SIZE];
            sprintf(msg, "%d", STOP);
            send(clients[i][1], msg, STOP);
        }
    }


    server_exit();
}

void sigint_handler(){
    stop();
}

int get_id_by_name(char* name){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][0] != -1){
            if(strcmp(clients_name[i], name) == 0) return i;
        }
    }
    return -1;
}

// dodaje nowego klienta do listy
void init_client(char* msg){
    if(client_counter >= MAX_CLIENTS){
        printf("Cannot add another client...\n");
        return;
    }

    int type;
    char *name = calloc(MSG_SIZE, sizeof(char));
    sscanf(msg, "%d %s", &type, name);

    int i = 0;
    while(i < MAX_CLIENTS){
        if(clients[i][0] == -1) break;
        i++;
    }
    clients[i][0] = 0;
    clients[i][1] = get_queue(name);
    client_counter++;
    clients_name[i] = calloc(100, sizeof(char));
    sprintf(clients_name[i], "%s", name);

    char confirmation[MSG_SIZE];
    sprintf(confirmation, "%s", "[SERVER CONFIRMATION]\n");

    send(clients[i][1], confirmation, INIT);
    printf("Sending confirmation - name: %s\n", name);
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
            printf("Client nr: %d, name: %s, status: %s\n", i, clients_name[i], status);
        }
    }
}

// łączy dwoch klientow w chatroom
void connect_to_client(char* msg){
    int client1, client2;
    sscanf(msg, "%d %d", &client1, &client2);

    char connect_msg[MSG_SIZE];
    sprintf(connect_msg, "%s", clients_name[client2]);
    send(clients[client1][1], connect_msg, CONNECT);

    sprintf(connect_msg, "%s", clients_name[client1]);
    send(clients[client2][1], connect_msg, CONNECT);

    clients[client1][0] = 1;
    clients[client2][0] = 1;
    connections[client1] = client2;
    connections[client2] = client1;

    printf(" Sent confirmation to clients -> %d - %d\n", client1, client2);
}

// rozłacza dwoch klientow
void disconnect(char* msg){
    int client1 = get_id_by_name(msg);
    int client2 = connections[client1];

    connections[client1] = -1;
    connections[client2] = -1;

    clients[client1][0] = 0;
    clients[client2][0] = 0;

    send(clients[client1][1], "", DISCONNECT);
    send(clients[client2][1], "", DISCONNECT);
    printf(" Sent confirmation to clients -> %d %s - %d %s\n", client1, clients_name[client1], client2, clients_name[client2]);
}

// obsluga dostarczanych zlecen
void handle_message(int type, char* message){
    switch(type){
        // 1
        case STOP:{
            printf("[STOP]\n");
            //stop_client(msg);
            break;
        }
        // 2
        case DISCONNECT:{
            printf("[DISCONNECT]");
            disconnect(message);
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
            connect_to_client(message);
            break;
        }
        // 5
        case INIT:{
            printf("[INIT]");
            init_client(message);
            break;
        }
        // 6
        case MESSAGE:{
            printf("[MESSAGE] - ");
            printf("%s\n", message);
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

    if((server_queue = create_queue(SERVER_NAME)) == -1){
        perror("failed to open");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, sigint_handler);

    printf("[SERVER] name: %s\n", SERVER_NAME);

    while(1){
        char* message = calloc(MSG_SIZE, sizeof(char));
        unsigned int type;
        receive(server_queue, message, &type);
        handle_message(type, message);
    }

}