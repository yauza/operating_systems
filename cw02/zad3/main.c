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

// funkcja sprawdzająca czy dana liczba jest kwadratem liczby całkowitej
int check_square(int n){
    int i = (int)sqrt((double) n);
    return i*i == n;
}

// funkcja sprawdzająca czy cyfra dziesiątek danej liczby wynosi 0 lub 7
int check_number_of_tens(int n){
    if(n < 10) return 0;
    int r = n%100 / 10;
    return r == 0 || r == 7;
}

// funkcja sprawdzająca parzystosc liczby
int check_parity(int n){
    return n%2 == 0;
}


void write_numbers_to_files_lib(){
    FILE* fp, *a, *b, *c;
    fp = fopen("dane.txt", "r");
    a = fopen("al.txt", "w");
    b = fopen("bl.txt", "w");
    c = fopen("cl.txt", "w");

    if(fp == NULL || a == NULL || b == NULL || c == NULL){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char* verse = calloc(MAX_ROW, sizeof(char));
    char c1;
    int flag = 0;
    int counter_of_even = 0;

    while(fread(&c1, 1, 1, fp) == 1){
        if(c1 == '\n'){
            int number = atoi(verse);
            // dodajemy "\n" ponieważ nie może on być uwzględniny podczas konwersji na int
            strcat(verse, "\n");

            if(check_parity(number)) counter_of_even++;

            if(check_number_of_tens(number)) fwrite(verse, 1, strlen(verse), b);

            if(check_square(number)) fwrite(verse, 1, strlen(verse), c);


            free(verse);
            flag = 1;
            verse = calloc(MAX_ROW, sizeof(char));
        }
        if(flag == 0) strcat(verse, &c1);
        flag = 0;

    }


    fwrite("Liczb parzystych jest ", 1, 22, a);
    char str[10];
    // int to char
    sprintf(str, "%d", counter_of_even);
    fwrite(str, 1, strlen(str), a);

    fclose(a);
    fclose(b);
    fclose(c);
    fclose(fp);
}


void write_numbers_to_files_sys(){
    int fp, a, b, c;
    fp = open("dane.txt", O_RDONLY);
    // dodatek 0666 w funkcji open() daje odpowiednie uprawnienia do pliku,
    // pozwalające na jego edycję i odczytanie
    a = open("as.txt", O_WRONLY|O_CREAT, 0666);
    b = open("bs.txt", O_WRONLY|O_CREAT, 0666);
    c = open("cs.txt", O_WRONLY|O_CREAT, 0666);

    if(fp < 0 || a < 0 || b < 0 || c < 0){
        perror("File opening error: ");
        exit(EXIT_FAILURE);
    }

    char* verse = calloc(MAX_ROW, sizeof(char));
    char c1;
    int flag = 0;
    int counter_of_even = 0;

    while(read(fp, &c1, 1) == 1){
        if(c1 == '\n'){
            int number = atoi(verse);
            strcat(verse, "\n");

            if(check_parity(number)) counter_of_even++;

            if(check_number_of_tens(number)) write(b, verse, strlen(verse));

            if(check_square(number)) write(c, verse, strlen(verse));

            free(verse);
            flag = 1;
            verse = calloc(MAX_ROW, sizeof(char));
        }
        if(flag == 0) strcat(verse, &c1);
        flag = 0;

    }

    write(a, "Liczb parzystych jest ", 22);
    char str[10];
    sprintf(str, "%d", counter_of_even);
    write(a, str, strlen(str));

    close(a);
    close(b);
    close(c);
    close(fp);
}


int main(int argc, char** argv){

    FILE* time_report = fopen("pomiar_zad_3.txt", "a");

    //lib
    printf("Writing results to files using lib functions... \n");
    start_counting_time();
    write_numbers_to_files_lib();
    write_time_results_to_file(time_report, "LIB FUNCTIONS");

    //sys
    printf("Writing results to files using sys functions... \n");
    start_counting_time();
    write_numbers_to_files_sys();
    write_time_results_to_file(time_report, "SYS FUNCTIONS");

    fclose(time_report);

    return 0;
}



