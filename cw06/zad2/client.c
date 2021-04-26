#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include "common.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

char* name;
int private_queue, server_queue, chat_queue;
int id;

void handle_message(int type);
void sigint_handler();
void stop();

// zamyka i usuwa swoją kolejke
void client_exit(){
    close_queue(server_queue);
    close_queue(private_queue);
    delete_queue(name);
    exit(0);
}

void stop(){
    char msg[MSG_SIZE];
    sprintf(msg, "%d %d", STOP, id);
    send(server_queue, msg, STOP);
    client_exit();
}

void sigint_handler(){
    stop();
}

// wysyla komunikat z prosba o dodanie do listy klientow
void init(){
    char msg[MSG_SIZE];
    sprintf(msg, "%d %s", INIT, name);

    send(server_queue, msg, INIT);

    char msg_rec[MSG_SIZE];
    unsigned int type;
    receive(private_queue, msg_rec, &type);

    sscanf(msg_rec, "%d %d", &type, &id);
    printf("Client registered with id: %d, name: %s\n", id, name);
}

// tworzy chatroom
void connect_to_other_client(char* msg){
    chat_queue = get_queue(msg);

    printf("\n***ENTERED THE CHAT***\n");
    printf("name: %s, queue_id: %d\n", msg, chat_queue);
}

// zamyka chatroom
void disconnect(){
    chat_queue = -1;
    printf("***LEFT THE CHAT***\n");
}

// rejestruje komunikat z serwera (innego klienta)
void notify()
{
    struct sigevent ev;
    ev.sigev_notify = SIGEV_SIGNAL;
    ev.sigev_signo = SIGUSR1;

    get_notification(private_queue, &ev);
}

// obsluguje przychodzące komunikaty
void handle_server_messages(){
    char *msg = calloc(MSG_SIZE, sizeof(char));
    unsigned int type;

    receive(private_queue, msg, &type);

    switch(type) {
        case STOP: {
            printf("[STOP] Server stopped working.\n");
            client_exit();
            break;
        }
        case DISCONNECT: {
            disconnect();
            break;
        }
        case LIST: {
            printf("[LIST]");
            break;
        }
        case CONNECT: {
            printf("[CONNECT]");
            connect_to_other_client(msg);
            break;
        }
        case MESSAGE: {
            printf("[MESSAGE] - ");
            printf("%s\n", msg);
            break;
        }
        default: {
            printf("Message without specified type...");
            break;
        }
    }


    notify();
}


void handle_message(int type){
    switch(type){
        case STOP:{
            printf("[STOP]\n");
            stop();
            break;
        }
        case DISCONNECT:{
            printf("[DISCONNECT]\n");
            send(server_queue, name, DISCONNECT);
            break;
        }
        case LIST:{
            send(server_queue, "", LIST);
            break;
        }
        case CONNECT:{
            printf("[CONNECT]");
            int my_id, client_id;
            scanf("%d %d", &my_id, &client_id);
            char msg[MSG_SIZE];
            sprintf(msg, "%d %d", my_id, client_id);
            send(server_queue, msg, CONNECT);
            break;
        }
        case MESSAGE:{
            printf("[MESSAGE] - ");
            char msg[MSG_SIZE];
            scanf("%s", msg);

            if(chat_queue != -1){
                send(chat_queue, msg, MESSAGE);
            } else send(server_queue, msg, MESSAGE);
            break;
        }
        default:{
            break;
        }

    }
}


int main(int argc, char** argv){
    srand(time(NULL));
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, handle_server_messages);
    chat_queue = -1;
    name = generate_random_name(5);

    if((private_queue = create_queue(name)) == -1){
        perror("client with this queue exists");
        exit(EXIT_FAILURE);
    }
    if((server_queue = get_queue(SERVER_NAME)) == -1){
        perror("failed to open server queue");
        exit(EXIT_FAILURE);
    }

    init();
    notify();

    int buff;

    while(1){
        printf("TYPE A MESSAGE: \n");
        scanf("%d", &buff);
        handle_message(buff);
    }

}