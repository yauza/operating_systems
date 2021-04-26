#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define ARGS 5
#define PROGRAMS 5



int main(int argc, char** argv){
    if(argc < 2){
        perror("Wrong number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    FILE* fp;
    if(argc == 2){
        char cmd[100] = "mail | ";
        strcat(cmd, argv[1]);
        //fp = popen(cmd, );

    }else{
        char cmd[255] = "echo \"";
        // tresc
        strcat(cmd, argv[3]);
        strcat(cmd, "\" | mail -s \"");
        // tytuł
        strcat(cmd, argv[2]);
        strcat(cmd, "\" ");
        // adres email
        strcat(cmd, argv[1]);

        fp = popen(cmd, "w");

    }

    pclose(fp);
    return 0;
}

