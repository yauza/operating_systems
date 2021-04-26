#include "lib.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000
#define MAX_ROW 255


int count_verses_file(char* file){
    FILE *fp;
    char str[MAX_ROW];
    fp = fopen(file, "r");
    if(fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    int counter = 0;
    while(fgets(str, MAX_ROW, fp) != NULL) counter++;

    fclose(fp);
    return counter;
}


char** get_verses(char* file_name, int file_size){
    FILE *fp;
    char str[MAX_ROW];
    fp = fopen(file_name, "r");
    if(fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char** verses = calloc(file_size, sizeof(char*));
    int i = 0;

    while(fgets(str, MAX_ROW, fp) != NULL){
        verses[i] = calloc(MAX_ROW, sizeof(char));
        strcpy(verses[i], str);
        i++;
    }

    return verses;
}


// funkcja mergująca tekst z dwóch plików
char** merge_verses(char** verses_a, char** verses_b, int size_a, int size_b){
    int i = 0, j = 0, k = 0;
    char** result = calloc(size_a + size_b, sizeof(char*));
    while(i < size_a && j < size_b){
        result[k] = calloc(MAX_ROW, sizeof(char));
        result[k+1] = calloc(MAX_ROW, sizeof(char));
        strcpy(result[k], verses_a[i]);
        strcpy(result[k+1], verses_b[j]);
        k += 2;
        i++;
        j++;
    }

    while(i < size_a){
        result[k] = calloc(MAX_ROW, sizeof(char));
        strcpy(result[k], verses_a[i]);
        k++;
        i++;
    }

    while(j < size_b){
        result[k] = calloc(MAX_ROW, sizeof(char));
        strcpy(result[k], verses_b[j]);
        k++;
        j++;
    }

    return result;
}


char* get_file_name(char* file, int size){
    int i = 0;
    while(file[i] != '.') i++;
    char* result = calloc(size, sizeof(char));
    memcpy(result, &file[0], i);
    return result;
}


char* merge_files(char* file_a, char* file_b, int new_file_number){
    char str[MAX_ROW];
    int size_a = count_verses_file(file_a), size_b = count_verses_file(file_b);

    // otwieramy i czytamy pliki a i b
    FILE *fp1, *fp2;
    fp1 = fopen(file_a, "r");
    if(fp1 == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fp2 = fopen(file_b, "r");
    if(fp2 == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char **verses_1, **verses_2;
    verses_1 = calloc(size_a, sizeof(char*));
    verses_2 = calloc(size_b, sizeof(char*));

    int i = 0, j = 0;

    // w pętlach zapisujemy ich zawartość (linia po linii)
    while(fgets(str, MAX_ROW, fp1) != NULL) {
        verses_1[i] = calloc(MAX_ROW, sizeof(char));
        strcpy(verses_1[i], str);
        i++;
        //printf("%s", str);
    }

    while(fgets(str, MAX_ROW, fp2) != NULL){
        verses_2[j] = calloc(MAX_ROW, sizeof(char));
        strcpy(verses_2[j], str);
        j++;
        //printf("%s", str);
    }
    fclose(fp1);
    fclose(fp2);

    // korzystając z funkcji pomocniczej mergujemy zawartość plików a i b
    char** to_write = merge_verses(verses_1, verses_2, size_a, size_b);

    char* new_file_name = calloc(MAX_ROW, sizeof(char));
    // tworzymy nazwę nowego (tymczasowego) pliku
    // troche plątaniny wynika z konieczności dodania
    // typu int do typu char*
    char temp[10];
    sprintf(temp, "%d", new_file_number);
    char* num_char = temp;
    strcat(new_file_name, num_char);
    strcat(new_file_name, "temp.txt");

    FILE* fp_temp;
    fp_temp = fopen(new_file_name, "w");
    if(fp_temp == NULL){
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    // zapisujemy zmergowane wersy do nowego pliku
    for(int i = 0; i < size_a + size_b; i++){
        fputs(to_write[i], fp_temp);
    }

    fclose(fp_temp);

    return new_file_name;

}

int check_if_integer(char* c){
    int i = 0;
    while(i < strlen(c)){
        if(isdigit(c[i])) i++;
        else return 0;
    }

    return 1;
}


char* get_substring(char* str, int start, int len){
    char* result = calloc(len + 1, sizeof(char));

    int i;
    for(i = 0; i < len; i++){
        *(result + i) = *(str + start - 1);
        str++;
    }
    *(result + i) = '\0';

    return result;

}


char** get_sequence(char* c){
    char** file_names = calloc(2, sizeof(char*));

    int i = 0, len = strlen(c);
    while(i < len) {
        if (c[i] == ':') break;
        i++;
    }

    if(i < 5 || len - i <= 5){
        perror("Wrong file names near 'merge_files'.\n");
        exit(EXIT_FAILURE);
    }

    // printf("%d %d %s\n", i, len, c);

    file_names[0] = get_substring(c, 1, i);
    file_names[1] = get_substring(c, i+2, len);

    return file_names;
}


void add_block_pointer(char* file_name, struct Block* block){
    int file_size = count_verses_file(file_name);
    char** file_verses = get_verses(file_name, file_size);
    block->verses = calloc(file_size, sizeof(char*));
    block->size = file_size;

    for(int i = 0; i < file_size; i++){
        block->verses[i] = file_verses[i];//calloc(MAX_ROW, sizeof(char));
        //strcpy(block->verses[i], file_verses[i]);
    }
}


void print_block(struct Block* block){
    for(int i = 0; i < block->size; i++){
        if(block->verses[i] == NULL) continue;
        printf("%s", block->verses[i]);
    }
}


void print_main_array(struct MainArray mainArray){
    if(mainArray.blocks == NULL) printf("Main array is empty.\n");
    for(int i = 0; i < mainArray.max_ind; i++){
        if(mainArray.blocks[i] == NULL) continue;
        printf("Block %d:\n", i);
        print_block(mainArray.blocks[i]);
    }
}


void remove_block(struct MainArray mainArray, int index){
//    int flag = 0, i;
//    for(i = 0; i < mainArray.size; i++) {
//        if (mainArray.blocks[i]->ind == index) {
//            flag = 1;
//            break;
//        }
//    }
//    if(flag == 0) {printf("Block with index %d does not exist", index); return;}


    for(int j = 0; j < mainArray.blocks[index]->size; j++){
        free(mainArray.blocks[index]->verses[j]);
    }
    free(mainArray.blocks[index]->verses);
    free(mainArray.blocks[index]);

    mainArray.blocks[index] = NULL;
    mainArray.size -= 1;
}


void remove_verse(struct MainArray mainArray, int block_index, int row_index){
//    int flag = 0, i;
//    for(i = 0; i < mainArray.size; i++) {
//        if(mainArray.blocks[i] == NULL) continue;
//        if (mainArray.blocks[i]->ind == block_index) {
//            flag = 1;
//            break;
//        }
//    }
//    if(flag == 0) {printf("Block with index %d does not exist", block_index); return;}
//
//    if(row_index >= mainArray.blocks[i]->size){printf("Row with index %d does not exist", row_index); return;}

    free(mainArray.blocks[block_index]->verses[row_index]);
    mainArray.blocks[block_index]->verses[row_index] = NULL;
}


struct MainArray create_main_array(int size){
    struct Block** blocks = calloc(size, sizeof(struct Block*));
    for(int i = 0; i < size; i++){
        blocks[i] = NULL;
    }

    struct MainArray temp;
    temp.blocks = blocks;
    temp.size = size;
    temp.max_ind = size;
    return temp;
}

