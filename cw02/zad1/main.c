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


void print_files_lib(char* file1, char* file2){
    FILE* fp1, *fp2;
    fp1 = fopen(file1, "r");
    fp2 = fopen(file2, "r");

    if(fp1 == NULL || fp2 == NULL){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char c1, c2;
    while(1){
        if(fread(&c1, 1, 1, fp1) == 1) {
            printf("%c", c1);
            while (fread(&c1, 1, 1, fp1) == 1 && c1 != '\n') printf("%c", c1);
            printf("\n");
        } else break;

        if(fread(&c2, 1, 1, fp2) == 1) {
            printf("%c", c2);
            while (fread(&c2, 1, 1, fp2) == 1 && c2 != '\n') printf("%c", c2);
            printf("\n");
        }else break;
    }

    while(fread(&c1, 1, 1, fp1) == 1) printf("%c", c1);

    while(fread(&c2, 1, 1, fp2) == 1) printf("%c", c2);

    fclose(fp1);
    fclose(fp2);
}


void print_files_sys(char* file1, char* file2){
    int fp1, fp2;
    fp1 = open(file1, O_RDONLY);
    fp2 = open(file2, O_RDONLY);

    if(fp1 < 0 || fp2 < 0){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char c1, c2;
    while(1){
        if(read(fp1, &c1, 1) == 1) {
            printf("%c", c1);
            while (read(fp1, &c1, 1) == 1 && c1 != '\n') printf("%c", c1);
            printf("\n");
        } else break;

        if(read(fp2, &c2, 1) == 1) {
            printf("%c", c2);
            while (read(fp2, &c2, 1) == 1 && c2 != '\n') printf("%c", c2);
            printf("\n");
        }else break;
    }

    while(read(fp1, &c1, 1) == 1) printf("%c", c1);

    while(read(fp1, &c2, 1) == 1) printf("%c", c2);

    close(fp1);
    close(fp2);
}


int main(int argc, char** argv){

    char* file1 = calloc(20, sizeof(char));
    char* file2 = calloc(20, sizeof(char));

	if(argc == 1){
	    printf("Input the file names: ");
	    scanf("%s", file1);
	    scanf("%s", file2);
	}else if(argc == 2) {
        printf("Wrong number of arguments.");
        exit(-1);
    }else{
	    file1 = argv[1];
	    file2 = argv[2];
	}

	FILE* time_report = fopen("pomiar_zad_1.txt", "a");

    //lib
    printf("Printing the files using lib functions: \n");
    start_counting_time();
    print_files_lib(file1, file2);
    write_time_results_to_file(time_report, "LIB FUNCTIONS");

    printf("\n\n");

    //sys
    printf("Printing the files using sys functions: \n");
    start_counting_time();
    print_files_sys(file1, file2);
    write_time_results_to_file(time_report, "SYS FUNCTIONS");

    return 0;
}
