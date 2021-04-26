#include "common.h"


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

char* generate_random_name(int length){
    char* res = calloc(length + 1 + 8, sizeof(char));
    res[0] = '/'; res[1] = 'q'; res[2] = 'u'; res[3] = 'e'; res[4] = 'u'; res[5] = 'e'; res[6] = 's'; res[7] = '-';
    for(int i = 8; i < length+9; i++){
        char l = 'a' + (rand()%26);
        res[i] = l;
    }
    res[length+9] = '\0';
    return res;
}

int send(mqd_t desc, char* msg_pointer, int type){
    return mq_send(desc, msg_pointer, strlen(msg_pointer), type);
}

int receive(mqd_t desc, char* msg_pointer, unsigned int* type_pointer){
    return mq_receive(desc, msg_pointer, MSG_SIZE, type_pointer);
}

int create_queue(char* name){
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE - 1;
    attr.mq_curmsgs = 0;

    return mq_open(name, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
}

void delete_queue(char* name){
    mq_unlink(name);
}

void close_queue(mqd_t descr){
    mq_close(descr);
}

int get_queue(char* name){
    return mq_open(name, O_WRONLY);
}

void get_notification(mqd_t desc, struct sigevent *s) { mq_notify(desc, s); }

