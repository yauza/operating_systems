#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/resource.h>
#include <unistd.h>

char* get_substring(char* str, int start, int len){
    char* result = calloc(len + 1, sizeof(char));

    int i;
    for(i = 0; i < len; i++){
        *(result + i) = *(str + start);
        str++;
    }
    *(result + i) = '\0';

    return result;

}

void read_cmd(char* cmd, struct Message* msg){
    char* t = get_substring(cmd, 0, 1);
    int type = atoi(t);
    msg->mtype = type;
    msg->order = type;
    sprintf(msg->message_text, "%s", get_substring(cmd, 2, strlen(cmd)-2));
}

int send(int queue_id, struct Message* msg){
    return msgsnd(queue_id, msg, MSG_SIZE, 0);
}

int receive(int queue_id, struct Message* msg){
    return msgrcv(queue_id, msg, MSG_SIZE, -100, IPC_NOWAIT);
}

int create_queue(int key){
    return msgget(key, IPC_CREAT | 0666);
}

int delete_queue(int queue_id){
    return msgctl(queue_id, IPC_RMID, NULL);
}

int get_queue(int key){
    return msgget(key, 0);
}

int queue_size(int queue_id){
    struct msqid_ds buffer;
    msgctl(queue_id, IPC_STAT, &buffer);
    return buffer.msg_qnum;
}
