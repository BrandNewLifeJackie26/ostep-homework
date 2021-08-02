#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed!");
        exit(EXIT_FAILURE);
    };

    // Fork a child
    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork failed!");
        exit(EXIT_FAILURE);
    }

    // Child process1 will link the write end of the pipe
    // with the stdout
    // 
    // ref: dup2()
    if (cpid == 0) {
        printf("Child1 created, pid=%d\n", getpid());

        // Close the read end first
        close(pipefd[0]);

        // Connect stdout with write end
        if (dup2(pipefd[1], STDOUT_FILENO) != STDOUT_FILENO) {
            perror("dup2 failed!");
            _exit(EXIT_FAILURE);
        }
        close(pipefd[1]);

        printf("This is a message sent by pipe!");
        return 0;
    }

    // Parent process will fork a new child process
    else {
        cpid = fork();
        if (cpid == -1) {
            perror("fork failed!");
            exit(EXIT_FAILURE);
        }

        // Child process 2 will connect read end of the pipe
        // to stdin to read the content out of pipe
        if (cpid == 0) {
            printf("Child2 created, pid=%d\n", getpid());

            // Close the write end first
            close(pipefd[1]);

            // Connect stdin with read end
            if (dup2(pipefd[0], STDIN_FILENO) != STDIN_FILENO) {
                perror("dup2 failed!");
                _exit(EXIT_FAILURE);
            }
            close(pipefd[0]);

            // Read from stdin and print it out to stdout (real stdout)
            // Q: Why blocking by read() if reading one by one?
            // A: the other child has been killed, so there is no way
            //  for this end to tell the state -> thus wait/ block
            char buf[BUFSIZ];
            read(STDIN_FILENO, buf, BUFSIZ);
            printf("%s\n", buf);

            return 0;
        }
    }
    
    // Wait for all child processes
    while ((cpid = wait(NULL)) != -1) {
        printf("Child exits, pid=%d\n", cpid);
    }
    printf("Parent exits!\n");

    return 0;
}
