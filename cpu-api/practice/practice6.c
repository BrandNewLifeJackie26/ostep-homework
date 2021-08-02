#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * waitpid() can designate the child pid the parent waits for.
 */
int main(int argc, char *argv[]) {
    pid_t cpid = fork();

    if (cpid == -1) {
        perror("fork failed!");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        printf("In child process\n");
        // waitpid(-1, NULL, 0) is the same as wait(NULL)
    } else {
        // wait for untraced and continued children
        pid_t rtn_pid = waitpid(cpid, NULL, WUNTRACED | WCONTINUED);
        printf("Waited for pid %d, returned value: %d\n", cpid, rtn_pid);
    }
    exit(EXIT_SUCCESS);
}