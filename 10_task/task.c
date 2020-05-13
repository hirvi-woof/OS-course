#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    pid_t pid, werr;
    int err;
    int status = 0;
    if (argc < 2) {
        err = printf("error - not enough arguments; usage: %s prog_name\n", argv[0]);
        exit(-1);
    }
    pid = fork();
    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("bad name\n");
        exit(-1);
    }
    else if (pid == -1)
    {
        perror("can't create child process\n");
        exit(-1);
    }
    if(wait(&status) == -1)
    {
        perror("waiting for a child failed");
        exit(-1);
    }
    printf("program returned %d\n", WEXITSTATUS(status));
    exit(0);
}
