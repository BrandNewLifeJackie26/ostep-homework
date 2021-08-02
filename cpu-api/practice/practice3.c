#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Pipe: Data written to the write end of
 * the pipe is buffered by the kernel until it is read from the read
 * end of the pipe.
 * 
 * The commented codes won't work because write() in two processes may run in any order.
 */
// int main(int argc, char *argv[]) {
//     int pipefd[2];
//     pid_t cpid;
//     char buf;

//     // Create pipe
//     if (pipe(pipefd) == -1) {
//         perror("pipe failed!");
//         exit(EXIT_FAILURE);
//     }

//     cpid = fork();
//     if (cpid == -1) {
//         perror("fork failed!");
//         exit(EXIT_FAILURE);
//     }

//     // Child will wait until all chars have been piped from parent
//     if (cpid == 0) {
//         // Close write end of the pipe
//         close(pipefd[1]);

//         // Read until EOF
//         while (read(pipefd[0], &buf, 1) > 0) {
//             write(STDOUT_FILENO, &buf, 1);
//         }
//         write(STDOUT_FILENO, "\n", 1);
//         close(pipefd[0]);
        
//         // https://stackoverflow.com/questions/5422831/what-is-the-difference-between-using-exit-exit-in-a-conventional-linux-fo
//         _exit(EXIT_SUCCESS);
//     }

//     // Parent will pass the words to print out in child first,
//     // and then continue with its own word
//     else {
//         close(pipefd[0]);
//         write(pipefd[1], "hello", strlen("hello"));
//         close(pipefd[1]);

//         printf("goodbye\n");
//         wait(NULL);
//         exit(EXIT_SUCCESS);
//     }
// }

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t cpid;
    char buf;

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed!");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork failed!");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        // Close read end of the pipe
        close(pipefd[0]);
        printf("hello\n");

        // Inform parent to write
        write(pipefd[1], "c", 2);
        close(pipefd[1]);
        
        _exit(EXIT_SUCCESS);
    }
    else {
        close(pipefd[1]);
        
        // Block until child writes into the pipe
        read(pipefd[0], &buf, 1);
        close(pipefd[0]);

        printf("goodbye\n");
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}