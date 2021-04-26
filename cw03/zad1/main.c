#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv){

	if(argc < 2) {
	    printf("Wrong number of arguments. \n");
	    exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
    pid_t pid = fork();
    int last_pid = -1;
	for(int i = 0; i < n; i++){
	    if(pid == 0){ //warunek oznaczający, że jesteśmy w procesie potomnym
	        if(last_pid != (int)getpid()) { //wypisujemy komunikat <=> kiedy jest to nowy proces, poprzedni już wypisano
                printf("Proces potomny pid: %d, pid rodzica: %d\n", (int) getpid(), (int) getppid());
                last_pid = (int) getpid();
            }
	    }else{ //jeżeli jesteśmy w procesie macierzystym to tworzymy kolejny proces
            pid = fork();
	    }
	}


	return 0;
}
