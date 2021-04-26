#ifndef lib_h
#define lib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MainArray{
    struct Block** blocks;
    int size;
    int max_ind;
};

struct Block{
    int ind;
    int size;
    char** verses;
};

//funkcja tworząca tablicę główną
struct MainArray create_main_array(int size);

//funkcja dodająca blok do tablicy głównej
//void add_block(char** verses, int size, struct MainArray mainArray);

//funkcja scalająca dwa pliki i tworząca nowy, zwracająca nazwę nowego pliku
char* merge_files(char* file_a, char* file_b, int new_file_number);

//funkcja używana do sprawdzenia czy argument jest liczbą
int check_if_integer(char* c);

//funkcja używana do sprawdzenie czy argument (przy merge_files) jest poprawną
//sekwencją plików, tzn. filea.txt:fileb.txt oraz pozyskania ich nazw
char** get_sequence(char* c);

//funkcja zwracająca liczbę wierszy w bloku
int count_verses_block(struct Block block);
int count_verses_file(char* file);

//dodaje wskaźniki bloku na wiersze pliku
void add_block_pointer(char* file_name, struct Block* block);

//funkcja usuwająca dany blok
void remove_block(struct MainArray mainArray, int index);

//funkcja usuwająca dany wiersz z bloku
void remove_verse(struct MainArray mainArray, int block_index, int row_index);

//funkcja wypisująca zmergowane pliki (z uwzględnieniem aktualizacji)
void print_merged_files(struct Block* main_array);

//wypisuje zawartość tablicy głównej
void print_main_array(struct MainArray mainArray);

//wypisuje zawartość bloku
void print_block(struct Block* block);

#endif //ZAD1_LIB_H
