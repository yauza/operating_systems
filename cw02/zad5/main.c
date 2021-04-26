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



void copy_the_file_lib(char* read_file, char* write_file){
    FILE* fr, *fw;
    fr = fopen(read_file, "r");
    fw = fopen(write_file, "w");


    if(fr == NULL || fw == NULL){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char c;
    char eol = '\n';
    int counter = 0;

    // Jeżeli dojdzie do rozmowy na temat programu
    // początkowy zamysł był podobny do zadania 3 i 4, czyli
    // konkatenacja do stringa verse kolejnych charów
    // W momencie kiedy w okolicach tej operacji był zwiększany licznik
    // to kopiowanie sie psuło
    // nie rozumiałem tego więc warto zapytać

    while(fread(&c, 1, 1, fr) == 1){
        counter++;
        fwrite(&c, 1, 1, fw);
        if(counter == 50){
            counter = 0;
            fwrite(&eol, 1, 1, fw);
        }

        if(c == eol) counter = 0;

    }

    fclose(fr);
    fclose(fw);
}


void copy_the_file_sys(char* read_file, char* write_file){
    int fw, fr;
    fr = open(read_file, O_RDONLY);

    // dodatek 0666 w funkcji open() daje odpowiednie uprawnienia do pliku,
    // pozwalające na jego edycję i odczytanie
    fw = open(write_file, O_WRONLY|O_CREAT, 0666);

    if(fr < 0 || fw < 0){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char c;
    char eol = '\n';
    int counter = 0;

    while(read(fr, &c, 1) == 1) {
        write(fw, &c, 1);
        counter++;
        if(counter == 50){
            write(fw, &eol, 1);
            counter = 0;
        }
        if(c == eol) counter = 0;
    }

    close(fr);
    close(fw);
}


int main(int argc, char** argv){

    if(argc < 3){
        perror("Number of arguments is too low.\n");
        exit(EXIT_FAILURE);
    }

    char* read_file = calloc(20, sizeof(char));
    char* write_file = calloc(20, sizeof(char));

    read_file = argv[1];
    write_file = argv[2];

    FILE* time_report = fopen("pomiar_zad_5.txt", "a");

    //lib
    printf("Copying the file using lib functions... \n");
    start_counting_time();
    copy_the_file_lib(read_file, write_file);
    write_time_results_to_file(time_report, "LIB FUNCTIONS");

    //sys
    printf("Copying the file using sys functions... \n");
    start_counting_time();
    copy_the_file_sys(read_file, write_file);
    write_time_results_to_file(time_report, "SYS FUNCTIONS");

    fclose(time_report);

    return 0;
}



