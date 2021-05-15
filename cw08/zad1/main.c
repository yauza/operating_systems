#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_LINE 255
#define SPACE " \t\r\n"

int threads_number;
int width, height;
int **image, **res_image;

void init(){
    image = calloc(height, sizeof(int*));
    for(int i = 0; i < height; i++) image[i] = calloc(width, sizeof(int));

    res_image = calloc(height, sizeof(int*));
    for(int i = 0; i < height; i++) res_image[i] = calloc(width, sizeof(int));
}


void open_file(char* file_name){
    FILE* fp = fopen(file_name, "r");
    if(fp == NULL){
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }

    char buff[MAX_LINE];
    // pominięcie nagłówka
    fgets(buff, MAX_LINE, fp);
    // pominięcie komentarza
    fgets(buff, MAX_LINE, fp);
    fgets(buff, MAX_LINE, fp);
    char* temp = buff;
    width = atoi(strtok_r(temp, SPACE, &temp));
    height = atoi(strtok_r(temp, SPACE, &temp));

    init();

    int i = 0, j = 0;
    while(fgets(buff, MAX_LINE, fp) != NULL){
        char* val = strtok(buff, SPACE);
        while(val != NULL){
            image[i][j] = atoi(val);
            j++;
            if(j == width){
                i++;
                if(i == height) break;
                j = 0;
            }
            val = strtok(NULL, SPACE);
        }
    }

    fclose(fp);
}

void save_file(char* file_name){
    FILE* fp = fopen(file_name, "w");
    if(fp == NULL){
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "P2\n%d %d\n%d\n", width, height, 255);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fprintf(fp, "%d ", res_image[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void* block(void* arg){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    int* r = arg;
    int k = *r;
    int c = ceil(width / threads_number);
    for(int i = (k - 1) * c; i <= (k * c) - 1; i++){
        for(int j = 0; j < height; j++){
            res_image[j][i] = 255 - image[j][i];
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double* time = calloc(1, sizeof(double));
    *time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000.0;
    pthread_exit(time);
}

void* numbers(void* arg){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    int* r = arg;
    int k = *r;
    int c = ceil(255 / threads_number);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image[i][j] >= (k - 1) * c && (image[i][j] <= (k * c) - 1 || k == threads_number)) {
                res_image[i][j] = 255 - image[i][j];
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double* time = calloc(1, sizeof(double));
    *time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000.0;
    pthread_exit(time);
}


int main(int argc, char** argv){
    if(argc < 5){
        perror("Wrong number of arguments.\n");
        exit(EXIT_FAILURE);
    }

    threads_number = atoi(argv[1]);
    int flag;
    if(strcmp(argv[2], "numbers") == 0) flag = 0;
    else if(strcmp(argv[2], "block") == 0) flag = 1;
    else{
        perror("Second argument must be either \"numbers\" or \"block\".\n");
        exit(EXIT_FAILURE);
    }
    char* file_name = argv[3];
    char* res_file = argv[4];

    open_file(file_name);

    FILE* file_time = fopen("times.txt", "a");
    if(file_time == NULL){
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file_time, "%s %d %d --- threads number: %d\n", file_name, width, height, threads_number);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    pthread_t* threads = calloc(threads_number, sizeof(pthread_t));
    int* args = calloc(threads_number, sizeof(int));
    for(int i = 0; i < threads_number; i++){
        args[i] = i + 1;
        if(flag) {
            pthread_create(&threads[i], NULL, block, (void*)&args[i]);
        }else{
            pthread_create(&threads[i], NULL, numbers, (void*)&args[i]);
        }
    }


    for(int i = 0; i < threads_number; i++){
        double* ret_val;
        pthread_join(threads[i], (void*)&ret_val);
        printf("[%d] -> %lf\n", i + 1, *ret_val);
        fprintf(file_time, "[%d] -> %lf\n", i + 1, *ret_val);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double* time = calloc(1, sizeof(double));
    *time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000.0;
    printf("[MAIN THREAD] -> %lf\n", *time);
    fprintf(file_time, "[MAIN THREAD] -> %lf\n\n\n", *time);

    save_file(res_file);

    return 0;
}

