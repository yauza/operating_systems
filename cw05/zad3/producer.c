#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{

    if (argc < 5)
    {
        fprintf(stderr, "Wrong arguments number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    char *pipe = argv[1];
    int row = atoi(argv[2]);
    char *file = argv[3];
    int N = atoi(argv[4]);

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }


    int pipef = open(pipe, O_WRONLY);
    if (pipef < 0)
    {
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }

    char str[N];
    while (fgets(str, N, fp) != NULL)
    {
        char message[N + 20];
        sprintf(message, "%d-%s\n", row, str);
        write(pipef, message, strlen(message));
        sleep(1);
    }

    close(pipef);
    fclose(fp);
    return 0;
}

