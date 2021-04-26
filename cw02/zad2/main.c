#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <string.h>

#define MAX_ROW 255

clock_t start_time, end_time;
struct tms start_cpu, end_cpu;

void start_counting_time(){ start_time = times(&start_cpu); }
void end_counting_time() { end_time = times(&end_cpu); }

void write_time_results_to_file(FILE* file, char* function_name){
    end_counting_time();

    int clock_tics = sysconf(_SC_CLK_TCK);
    double real = (double)(end_time - start_time) / clock_tics;
    double user = (double)(end_cpu.tms_utime - start_cpu.tms_utime) / clock_tics;
    double system = (double)(end_cpu.tms_stime - start_cpu.tms_stime) / clock_tics;

    fprintf(file, "%s\nReal: %15f\n User: %15f\n System: %15f\n", function_name, real, user, system);
}


void print_verses_lib(char* file, char c){
    FILE* fp;
    fp = fopen(file, "r");

    if(fp == NULL){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char* verse = calloc(MAX_ROW, sizeof(char));
    char c1;
    int flag = 0;

    while(fread(&c1, 1, 1, fp) == 1){
        if(c1 == c) flag = 1;
        strcat(verse, &c1);
        if(c1 == '\n'){
            if(flag){
                printf("%s", verse);
            }
            free(verse);
            verse = calloc(MAX_ROW, sizeof(char));
            flag = 0;
        }

    }

    fclose(fp);
}


void print_verses_sys(char* file, char c){
    int fp;
    fp = open(file, O_RDONLY);

    if(fp < 0){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char* verse = calloc(MAX_ROW, sizeof(char));
    char c1;
    int flag = 0;

    while(read(fp, &c1, 1) == 1){
        if(c1 == c) flag = 1;
        strcat(verse, &c1);
        if(c1 == '\n'){
            if(flag){
                printf("%s", verse);
            }
            free(verse);
            verse = calloc(MAX_ROW, sizeof(char));
            flag = 0;
        }

    }

    close(fp);
}


int main(int argc, char** argv){

    if(argc < 3) {
        printf("Wrong number of arguments.");
        exit(-1);
    }

    char* file = calloc(20, sizeof(char));
    char c;

    c = *argv[1];
    file = argv[2];

    FILE* time_report = fopen("pomiar_zad_2.txt", "a");

    //lib
    printf("Printing verses using lib functions: \n");
    start_counting_time();
    print_verses_lib(file, c);
    write_time_results_to_file(time_report, "LIB FUNCTIONS");

    printf("\n\n");

    //sys
    printf("Printing verses using sys functions: \n");
    start_counting_time();
    print_verses_sys(file, c);
    write_time_results_to_file(time_report, "SYS FUNCTIONS");

    return 0;
}


