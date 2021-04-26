#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


char* get_substring(char* str, int start, int len){
    char* result = calloc(len + 1, sizeof(char));

    int i;
    for(i = 0; i < len; i++){
        *(result + i) = *(str + start);
        str++;
    }
    *(result + i) = '\0';

    return result;
}


void write_to_ith_line(int row, FILE* fp, char* buffer){
    int counter = 1;
    char str[255];
    while(fgets(str, sizeof(str), fp)){
            if(counter == row){
                char* newline = calloc(255, sizeof(char));
                printf("old line: %s\n", str);
                strcat(newline, str);
                strcat(newline, buffer);
                printf("new line: %s\n", newline);
                fputs(newline, fp);
                break;
            }
            counter++;
    }

}


int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    char *pipe = argv[1];
    char *file = argv[2];
    int N = atoi(argv[3]);
    FILE *fp = fopen(file, "w+");
    if (fp == NULL)
    {
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }

    FILE *pipef = fopen(pipe, "r");
    if (pipef == NULL)
    {
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }
    char str[N];


    while (fgets(str, N, pipef) != NULL){
        // int row = atoi(&str[0]);
        printf("%s", str);

    }

    fclose(pipef);
    fclose(fp);
    return 0;
}

