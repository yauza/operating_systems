#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char *c[] = {"./consumer", "fifo", "c.txt", "10", NULL};
    char *p1[] = {"./producer", "fifo", "1", "p1.txt", "5", NULL};
    char *p2[] = {"./producer", "fifo", "2", "p2.txt", "5", NULL};
    char *p3[] = {"./producer", "fifo", "3", "p3.txt", "5", NULL};


    mkfifo("fifo", S_IRUSR | S_IWUSR);

    pid_t pid_tab[4];
    if ((pid_tab[0] = fork()) == 0)
        execvp(c[0], c);

    if ((pid_tab[1] = fork()) == 0)
        execvp(p1[0], p1);

    if ((pid_tab[2] = fork()) == 0)
        execvp(p2[0], p2);

    if ((pid_tab[3] = fork()) == 0)
        execvp(p3[0], p3);


    for (int i = 0; i < 4; i++)
        waitpid(pid_tab[i], NULL, 0);

    return 0;
}

