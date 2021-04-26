#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "lib.h"


clock_t start_time, end_time;
struct tms start_cpu, end_cpu;
FILE* time_report;

void start_counting_time(){ start_time = times(&start_cpu); }
void end_counting_time() { end_time = times(&end_cpu); }

void write_time_results_to_file(FILE* file, char* function_name){
    end_counting_time();

    int clock_tics = sysconf(_SC_CLK_TCK);
    double real = (double)(end_time - start_time) / clock_tics;
    double user = (double)(end_cpu.tms_utime - start_cpu.tms_utime) / clock_tics;
    double system = (double)(end_cpu.tms_stime - start_cpu.tms_stime) / clock_tics;

    fprintf(file, "%s\nReal: %15f\n User: %15f\n System: %15f\n", function_name, real, user, system);
    //printf("%s\nReal: %15f\n User: %15f\n System: %15f\n", function_name, real, user, system);
}




int main(int argc, char** argv){

    if(argc < 2){
        printf("Number of arguments is too low. \n");
        exit(EXIT_FAILURE);
    }

    char** file_names;
    FILE* fp = fopen("time_report.txt", "a");
    if(fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    start_counting_time();
    pid_t pid = fork();
    int last_pid = -1, flag = 0, parent_pid;

    if(pid == 0) parent_pid = (int)getppid();
    else parent_pid = (int)getpid();

    for(int i = 1; i < argc; i++){
        if(pid == 0) {
            if(last_pid != (int)getpid()) {
                file_names = get_sequence(argv[i]);
                char *new_file_name = merge_files(file_names[0], file_names[1], i);
                last_pid = (int)getpid();
                printf("Utworzono plik o nazwie %s w procesie potomnym pid: %d, ppid: %d\n",
                                                                new_file_name, last_pid, (int)getppid());
            }
        }else{
            pid = fork();
            wait(NULL);
        }
    }
    wait(NULL);
    /***
     * Dlaczego używam wait()?
     * https://stackoverflow.com/questions/15183427/getpid-and-getppid-return-two-different-values/15183445
     * "What's happening is that the parent is terminating before the child runs. this leaves the child as an orphan and
     * it gets adopted by the root process with PID of 1"
     *
     * Funkcja wait powoduje, że proces macierzysty czeka aż proces potomny zakończy działanie.
     */

    if(getpid() == parent_pid) write_time_results_to_file(fp, "merging files");
    fclose(fp);

    return 0;
}


