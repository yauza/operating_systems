#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>


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

//funkcja sprawdzająca czy dany plik zawiera podany łańcuch
int check_if_contains_phrase(char* file_name, char* phrase){
    FILE* fp = fopen(file_name, "r");
    if(fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char str[strlen(phrase)];
    int i = 1, flag = 0;
    //zasada działania jak w zadaniu 4 zestawu 2
    //przechodzimy "okienkiem" po pliku i szukamy takiego
    //"okna" które jest równe naszemu łańcuchowi
    while(fread(&str, 1, strlen(phrase), fp) > 0){
        if(strcmp(str, phrase) == 0){
            flag = 1;
            break;
        }else{
            fseek(fp, i, 0);
            i++;
        }
    }

    fclose(fp);
    return flag;
}

//funkcja sprawdzająca czy plik ma rozszerzenie .txt
int check_if_text_file(char* file_name){
    if(strcmp(get_substring(file_name, strlen(file_name)-4, 4), ".txt") == 0) return 1;
    else return 0;
}


void search_directory(const char *name, char* phrase, int search_depth, const char* rel_name, int last_pid)
{
    if(search_depth < 0) return;

    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))){
        printf("Cannot open directory. Returning.\n");
        return;
    }

    pid_t pid;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            char relative_path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            //tworzymy nowy proces i w nim właśnie będziemy przeglądać kolejny katalog
            pid = fork();
            wait(NULL);
            if(pid != 0) continue;

            //zapisujemy nową ścieżkę (względną też)
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            snprintf(relative_path, sizeof(relative_path), "%s/%s", rel_name, entry->d_name);
            //wchodzimy do katalgou
            search_directory(path, phrase, search_depth-1, relative_path, last_pid);
            //po zakończeniu przeszukiwania katalogu proces potomny sie kończy
            exit(0);
        } else {
            if(check_if_text_file(entry->d_name)){
                char temp[1024];
                snprintf(temp, sizeof(temp), "%s/%s", name, entry->d_name);
                if(check_if_contains_phrase(temp, phrase)) {
                    //%*s dodaje taby
                    printf("%*s%s\n", 0, "", rel_name);
                    printf("%*s PID: %d  PPID: %d\n", 0, "", (int)getpid(), (int)getppid());
                    printf("%*s- %s\n", 4, "", entry->d_name);
                }
            }
        }
    }
    closedir(dir);
    exit(0);
}


int main(int argc, char** argv){

    if(argc < 4) {
        printf("Wrong number of arguments. \n");
        exit(EXIT_FAILURE);
    }

    char* dir_name = argv[1];
    char* phrase = argv[2];
    int search_depth = atoi(argv[3]), last_pid = getpid();
    printf("\nSTART PID: %d\n", (int)getpid());
    printf("**********\nListing files from %s...\n**********\n", dir_name);
    search_directory(dir_name, phrase, search_depth, "", last_pid);


    return 0;
}

