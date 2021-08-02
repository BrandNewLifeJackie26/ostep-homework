#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main() {
    int fd = open("practice2.txt", O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd == -1) {
        printf("Error when opening file, errno=%d", errno);
        exit(1);
    }

    pid_t pid = fork();
    if (pid < -1) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // child
        char *str1 = "Child writes!\n";
        write(fd, str1, strlen(str1));
    } else {
        // parent
        // wait(NULL);
        char *str2 = "Parent writes!\n";
        write(fd, str2, strlen(str2));
    }

    int close_rtn = close(fd);
    if (close_rtn == -1) {
        printf("Error when closing the file!");
        exit(1);
    }

    return 0;
}