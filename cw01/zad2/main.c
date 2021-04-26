#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>
#include "lib.h"

#define MAXCHAR 1000

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




int main(int argc, char **argv){

    if(argc < 2) {
        perror("Number of arguments is too low.\n");
        exit(EXIT_FAILURE);
    }



    int i = 1, number_of_files = atoi(argv[1]);
    int new_file_number = 0; //numer pliku tymczasowego
    int main_array_created = 0, block_number;

    struct MainArray mainArray;
    time_report = fopen("raport2.txt", "a");

    while(i < argc){

        // jeżeli argument to create_table
        if(strcmp(argv[i], "create_table") == 0){
            if(i+1 >= argc || !check_if_integer(argv[i+1])){
                perror("Bad argument near command 'create_table'.\n");
                exit(EXIT_FAILURE);
            }
            mainArray = create_main_array(atoi(argv[i+1]));
            main_array_created = 1;
            block_number = 0;

            for(int j = 0; j < mainArray.size; j++){
                mainArray.blocks[j] = calloc(1, sizeof(struct Block));
                mainArray.blocks[j]->ind = j;
            }

        }else if(strcmp(argv[i], "merge_files") == 0){
            char** file_names;

            start_counting_time();
            for(int j = 0; j < number_of_files; j++){
                file_names = get_sequence(argv[i + j + 1]);
                char* new_file_name = merge_files(file_names[0], file_names[1], new_file_number);
                new_file_number++;
                //
                // Jeżeli tablica główna istnieje to tworzymy bloki i ustawiamy
                // wskaźniki na wiersze w nowym pliku
                if(main_array_created){
                    if(block_number < mainArray.size){
                        add_block_pointer(new_file_name, mainArray.blocks[block_number]);
                        block_number++;
                    }else{
                        perror("Number of blocks is too little.\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            end_counting_time();
            write_time_results_to_file(time_report, "merge_files");

        }else if(strcmp(argv[i], "remove_block") == 0){
            if(main_array_created == 0){
                perror("Cannot remove block because main array is not created.\n");
                exit(EXIT_FAILURE);
            }
            if(i+1 >= argc || !check_if_integer(argv[i+1])){
                perror("Bad argument near command 'remove_block'.\n");
                exit(EXIT_FAILURE);
            }

            int block_index_to_remove = atoi(argv[i+1]);

            start_counting_time();
            remove_block(mainArray, block_index_to_remove);
            end_counting_time();
            write_time_results_to_file(time_report, "remove_block");

            if(mainArray.size == 0){
                main_array_created = 0;
            }

        }else if(strcmp(argv[i], "remove_row") == 0){
            if(main_array_created == 0){
                perror("Cannot remove row because main array is not created.\n");
                exit(EXIT_FAILURE);
            }

            if(i+2 >= argc || !check_if_integer(argv[i+1]) || !check_if_integer(argv[i+2])){
                perror("Bad argument near command 'remove_row'.\n");
                exit(EXIT_FAILURE);
            }

            int block_index = atoi(argv[i+1]), row_index = atoi(argv[i+2]);

            start_counting_time();
            remove_verse(mainArray, block_index, row_index);
            end_counting_time();
            write_time_results_to_file(time_report, "remove_row");

        }

        i++;
    }

    print_main_array(mainArray);

    return 0;
}

