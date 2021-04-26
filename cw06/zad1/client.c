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

key_t private_key, server_key, chat_key;
int private_queue_id, server_queue_id, chat_queue_id;

void handle_message(struct Message msg);
void sigint_handler();
void stop();

void clean_on_exit(){ delete_queue(private_queue_id); }

void connect_to_other_client(struct Message msg){
    chat_key = atoi(msg.message_text);
    chat_queue_id = get_queue(chat_key);

    printf("\n***ENTERED THE CHAT***\n");
    printf("key: %d, queue_id: %d\n", chat_key, chat_queue_id);
}

void disconnect(){
    chat_key = -1;
    chat_queue_id = -1;
    printf("***LEFT THE CHAT***\n");
}

void handle_message(struct Message msg){
    switch(msg.mtype){
        case STOP:{
            printf("[STOP] Server stopped working.\n");
            stop();
            break;
        }
        case DISCONNECT:{
            printf("[DISCONNECT]");
            disconnect();
            break;
        }
        case LIST:{
            printf("[LIST]");
            break;
        }
        case CONNECT:{
            printf("[CONNECT]");
            connect_to_other_client(msg);
            break;
        }
        case MESSAGE:{
            printf("[MESSAGE] - ");
            printf("%s", msg.message_text);
            break;
        }
        default:{
            printf("Message without specified type...");
            break;
        }

    }
}

void sigint_handler(){
    struct Message message;
    message.mtype = 1;
    message.client_key = private_key;
    if(send(server_queue_id, &message) < 0){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    exit(0);
}

void stop(){
    exit(0);
}

int main(int argc, char** argv){
    atexit(clean_on_exit);
    chat_key = -1;
    chat_queue_id = -1;
    signal(SIGINT, sigint_handler);
    server_key = ftok(getenv("HOME"), 1);

    if((server_queue_id = msgget(server_key, IPC_CREAT | 0666)) < 0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    private_key = ftok(getenv("HOME"), getpid());
    private_queue_id = msgget(private_key, IPC_CREAT | 0666);

    struct Message msg;
    // msg.mtype = 1;
    msg.client_key = private_key;

    printf("[CLIENT] private_key: %d, private_queue_id: %d\n", private_key, private_queue_id);

    char cmd[20];
    struct Message buffer;
    while(1){
        printf("TYPE A MESSAGE: \n");
        if (fgets(cmd, 20, stdin) == NULL) {
            printf("client: error reading your command\n");
            continue;
        }
        if (strcmp(cmd, "\n") != 0) {
            read_cmd(cmd, &msg);

            if (msg.mtype == MESSAGE) {
                if (chat_queue_id != -1) {
                    if (send(chat_queue_id, &msg) < 0) {
                        perror("msgsnd chat");
                        exit(EXIT_FAILURE);
                    }
                } else printf("You are not connected to any other client.\n");
            } else {
                if (send(server_queue_id, &msg) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
            }
            printf("|SENT|\n");
            if (msg.mtype == INIT) sleep(1);
            if (msg.mtype == STOP) {
                sleep(1);
                stop();
            }
            if (msg.mtype == CONNECT) sleep(1);

        }

        while (queue_size(private_queue_id) > 0) {
            if (receive(private_queue_id, &buffer) < 0) {
                perror("[CLIENT] message receive failed...\n");
                exit(EXIT_FAILURE);
            }
            handle_message(buffer);
        }


    }

}