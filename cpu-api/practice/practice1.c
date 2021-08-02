#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    int x = 100;

    pid_t pid = fork();
    if (pid < -1) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // child
        x += 10;
        printf("I am the child, pid is %d, this is the x value: %d\n", pid, x);
    } else {
        // parent
        wait(NULL);
        x -= 10;
        printf("I am the parent, pid is %d, this is the x value: %d\n", pid, x);
    }

    return 0;
}