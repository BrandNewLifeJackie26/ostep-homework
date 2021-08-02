#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t cpid;

    cpid = fork();
    if (cpid == -1) {
        perror("fork failed!");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        // Try wait -> value == -1 (failed to wait -> errno == ECHILD)
        int rtn_child = wait(NULL);
        printf("Child called wait and returns %d\n", rtn_child);
    } else {
        int rtn_parent = wait(NULL);
        printf("Child pid: %d, parent's wait returns %d\n", cpid, rtn_parent);
    }
    exit(EXIT_SUCCESS);
}