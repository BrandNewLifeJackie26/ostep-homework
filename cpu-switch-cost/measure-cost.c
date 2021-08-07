#define _GNU_SOURCE
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

#define errExit(msg) do {perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int main(int argc, char *argv[]) {
    struct timeval tv_start, tv_end;
    int times = 1000000;
    
    /**
     * Calculate cost of system calls
     */
    // Open file
    int fd = open("example.txt", O_RDONLY);

    // Start timer
    if (gettimeofday(&tv_start, NULL) == -1) errExit("gettimeofday failed!");

    // System calls
    for (int i = 0; i < times; i++) {
        char c;
        read(fd, &c, 0);
    }

    // End timer
    if (gettimeofday(&tv_end, NULL) == -1) errExit("gettimeofday failed!");
    
    // Output information
    long total_usec = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 + tv_end.tv_usec - tv_start.tv_usec;
    printf("System call time cost: Total = %ld microseconds, Each = %ld microseconds! \n",
        total_usec, total_usec / times);

    /**
     * Calculate time cost of context switch
     */
    int p2c_pipefd[2], c2p_pipefd[2];
    if (pipe(p2c_pipefd) == -1) errExit("pipe failed!");
    if (pipe(c2p_pipefd) == -1) errExit("pipe failed!");

    int cpid;
    char c;
    cpid = fork();
    if (cpid == -1) errExit("fork failed!");

    // Set the same CPU
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    // Child
    if (cpid == 0) {
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) errExit("sched_setaffinity failed!");

        for (int i = 0; i < times; i++) {
            write(c2p_pipefd[1], &c, 0);
            read(p2c_pipefd[0], &c, 0);
        }

        return 0;
    }

    // Parent
    // Start timer
    if (gettimeofday(&tv_start, NULL) == -1) errExit("gettimeofday failed!");

    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) errExit("sched_setaffinity failed!");
    for (int i = 0; i < times; i++) {
        read(c2p_pipefd[0], &c, 0);
        write(p2c_pipefd[1], &c, 0);
    }

    // End timer
    if (gettimeofday(&tv_end, NULL) == -1) errExit("gettimeofday failed!");
    
    // Output information
    total_usec = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 + tv_end.tv_usec - tv_start.tv_usec;
    printf("System call time cost: Total = %ld microseconds, Each = %ld microseconds! \n",
        total_usec, total_usec / times);

    // Wait for child
    if (wait(NULL) == -1) errExit("wait failed!");

    exit(EXIT_SUCCESS);
}