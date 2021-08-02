#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork failed!");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        close(STDOUT_FILENO);
        printf("After closing stdout, try to print something in child!\n");
    } else {
        wait(NULL);
        printf("After closing stdout, try to print something in parent!\n");
    }
    return 0;
}
