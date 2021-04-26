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
#include <math.h>

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


void rewrite_the_file_lib(char* read_file, char* write_file, char* n1, char* n2){
    FILE* fr, *fw;
    fr = fopen(read_file, "r");
    fw = fopen(write_file, "w");

    if(fr == NULL || fw == NULL){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char str[strlen(n1)];
    int i = 1;
    // przechodzimy "okienkiem" po pliku - jeżeli to okno będzie
    // odpowiadać n1, to zapisujemy n2 do pliku fw
    while(fread(&str, 1, strlen(n1), fr) > 0){
        if(strcmp(str, n1) == 0) {
            fwrite(n2, 1, strlen(n2), fw);
            i += strlen(n1);
        }else{
            // wracamy na pozycję poprzednią i zapisujemy kolejną literkę
            fseek(fr, i, 0);
            fwrite(&str[0], 1, 1, fw);
            i++;
        }

    }


    fclose(fr);
    fclose(fw);
}


void rewrite_the_file_sys(char* read_file, char* write_file, char* n1, char* n2){
    int fr, fw;
    fr = open(read_file, O_RDONLY);
    fw = open(write_file, O_WRONLY|O_CREAT, 0666);

    if(fr < 0 || fw < 0){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char str[strlen(n1)];
    int i = 1;
    while(read(fr, &str, strlen(n1)) > 0){
        if(strcmp(str, n1) == 0) {
            write(fw, n2, strlen(n2));
            i += strlen(n1);
        }else{
            lseek(fr, i, SEEK_SET);
            write(fw, &str[0], 1);
            i++;
        }
    }


    close(fr);
    close(fw);
}


int main(int argc, char** argv){

    if(argc < 5){
        perror("Number of arguments is too low.\n");
        exit(EXIT_FAILURE);
    }

    char* read_file = calloc(20, sizeof(char));
    char* write_file = calloc(20, sizeof(char));
    char* n1 = calloc(MAX_ROW, sizeof(char));
    char* n2 = calloc(MAX_ROW, sizeof(char));

    read_file = argv[1];
    write_file = argv[2];
    n1 = argv[3];
    n2 = argv[4];

    FILE* time_report = fopen("pomiar_zad_4.txt", "a");

    //lib
    printf("Writing results to files using lib functions... \n");
    start_counting_time();
    rewrite_the_file_lib(read_file, write_file, n1, n2);
    write_time_results_to_file(time_report, "LIB FUNCTIONS");

    //sys
    printf("Writing results to files using sys functions... \n");
    start_counting_time();
    rewrite_the_file_sys(read_file, write_file, n1, n2);
    write_time_results_to_file(time_report, "SYS FUNCTIONS");

    fclose(time_report);

    return 0;
}





