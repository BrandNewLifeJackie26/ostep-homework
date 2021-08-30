#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define errExit(msg) do {perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

/**
 * Make loops 
 * First argument: size of one block memory to be created in one loop
 * Second argument (optional): number of loops (default 5)
 */
int main(int argc, char const *argv[]) {
    // Only accept two or three command line argument
    if (argc != 3 && argc != 2) {
        errExit("Wrong number of arguments were given!");
    }

    // Loop several times (according to the second argument)
    // to allocate new heap memory
    int size = atoi(argv[1]);
    int loops = argc == 3 ? atoi(argv[2]) : 5;
    int **blocks = (int **) malloc(loops * sizeof(int *));

    // malloc
    for (int i = 0; i < loops; i++) {
        *(blocks+i) = malloc(size * sizeof(int));
        sleep(5);
    }

    // free
    for (int i = 0; i < loops; i++) {
        free(*(blocks+i));
    }
    free(blocks);

    return 0;
}
