#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define ARGS 5
#define PROGRAMS 5

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

// Funkcja wydobywająca polecenie z linijki -> usuwa napis 'skladnikn = '
char* get_command(char* str){
    char* rest = NULL;
    char* save = calloc(strlen(str), sizeof(char));
    strcpy(save, str);
    strtok_r(str, " ", &rest);
    char* res = get_substring(save, strlen(str) + 3, strlen(save) - strlen(str) - 3);
    return res;
}


// funkcja pomocnicza wypisująca tablicę z poleceniami
void print_args(char* arguments[PROGRAMS][ARGS]){
    for(int i = 0; i < PROGRAMS; i++){
        printf("%d: ", i);
        for(int j = 0; j < ARGS; j++){
            printf("%d: %s ", j, arguments[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------\n");
}

// funkcja uruchamiająca polecenia
void run_commands(char** commands, char* run){
    // tworzymy tablicę polecen wraz z argumentami
    char* arguments[PROGRAMS][ARGS];
    for(int i = 0; i < PROGRAMS; i++)
        for (int j = 0; j < ARGS; j++)
            arguments[i][j] = NULL;

    // porcja kodu od 86 do 115 linijki
    // służy do wyciągania poleceń oraz argumentów z
    // konstrukcji skladnik1 | skladnik2 | ...

   char* line = calloc(255, sizeof(char));
   char* tp = run;
   char* skl = strtok_r(run, " | ", &tp);
   while(skl != NULL){
       int space = (skl[0] == ' ') ? 1 : 0;
       int nr = atoi(get_substring(skl, 8+space, strlen(skl) - (8+space)));
       strcat(line, get_substring(commands[nr], 0, strlen(commands[nr])-1));
       skl = strtok_r(NULL, "|", &tp);
       if(skl != NULL) strcat(line, " | ");
   }

    char* temp = line;
    char* cmd = strtok_r(line, "|", &temp);
    int i = 0;
    while(cmd != NULL){
        char* cmd2 = cmd;
        char* arg = strtok_r(cmd2, " ", &cmd2);
        int j = 0;
        while(arg != NULL){
            arguments[i][j] = arg;
            arg = strtok_r(NULL, " ", &cmd2);
            j++;
        }

        cmd = strtok_r(NULL, "|", &temp);
        i++;
    }
    int number_of_programs = i;

    // tworzymy sobie pipe'y do komunikacji pomiędzy procesami

    int fd[PROGRAMS][2];
    for(int k = 0; k < number_of_programs-1; k++) {
        if(pipe(fd[k]) < 0){
            perror("Cannot make pipe.\n");
            exit(EXIT_FAILURE);
        }
    }

    // tworzymy nowe procesy potomne, w których będą uruchamiane kolejne polecenia
    for(int k = 0; k < number_of_programs; k++){
        pid_t pid = fork();
        if(pid == 0){
            // jeżeli nie jest to pierwsze polecenie, to podmieniamy wejście
            if(k > 0) dup2(fd[k - 1][0], STDIN_FILENO);
            // jeżeli nie jest to ostatnie polecenie, to podmieniamy wyjście
            if(k + 1 < number_of_programs) dup2(fd[k][1], STDOUT_FILENO);
            // zamykamy nieużywane deskryptory
            for(int j = 0; j < number_of_programs - 1; j++){
                close(fd[j][0]);
                close(fd[j][1]);
            }
            // wywołujemy polecenie
            execvp(arguments[k][0], arguments[k]);
            exit(0);

        }
    }

    // zamykamy deskryptory
    for(int j = 0; j < number_of_programs - 1; ++j){
        close(fd[j][0]);
        close(fd[j][1]);
    }

    // oczekujemy na zakończenie poleceń
    for(int j = 0; j < number_of_programs; ++j){
        wait(0);
    }
}


int main(int argc, char** argv){
    if(argc < 2){
        perror("Wrong number of arguments.\n");
        exit(EXIT_FAILURE);
    }

    char* file = argv[1];
    FILE* fp = fopen(file, "r");
    if(fp == NULL){
        perror("Cannot open the file.\n");
        exit(EXIT_FAILURE);
    }

    char buff[255];
    int flag = 0, i = 1;
    char** commands = calloc(100, sizeof(char*));
    commands[0] = NULL;

    // czytamy w pliku kolejne linie
    while(fgets(buff, 255, fp) != NULL){
        if(strcmp(buff, "\n") == 0) { flag = 1; continue; }
        // póki flaga == 0, zapisujemy składniki do tablicy commands
        if(!flag) {
            commands[i] = get_command(buff);
            i++;
        // w przeciwnym przypadku uruchamiamy polecenia
        }else{
            run_commands(commands, buff);
        }
    }


    fclose(fp);

    return 0;
}