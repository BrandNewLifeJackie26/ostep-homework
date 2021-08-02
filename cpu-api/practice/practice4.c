#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// l - execl, execlp, execle: accepts a list of input args
// v - execv, execvp, execvpe: vector of args
// e - execle, execvpe: environment of new process image is specified via envp
// p - execlp, execvp, execvpe: duplicate the actions of the shell in searching for an executable
int main(int argc, char *argv[]) {
    pid_t cpid;

    cpid = fork();
    if (cpid == -1) {
        perror("fork failed!");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        int rc = execl("/bin/ls", "ls", (char *)NULL); // IMPORTANT: NULL as a terminator
        if (rc == -1) {
            perror("exec failed!");
            exit(EXIT_FAILURE);
        }
    } else {
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}