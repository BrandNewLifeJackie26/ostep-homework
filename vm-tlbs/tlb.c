#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define errExit(msg) do {perror(msg); exit(EXIT_FAILURE); \
                        } while (0)
#define PAGE_SIZE (1 << 20)

int main(int argc, char const *argv[])
{
    if (argc != 3) errExit("Please provide two arguments: number of pages and trials.");
    int numPages = atoi(argv[1]); // TODO: exception handling
    int numTrials = atoi(argv[2]);

    struct timeval start_tv, end_tv;
    
    // Start
    if (gettimeofday(&start_tv, NULL) == -1) errExit("gettimeofday failed!\n");
    printf("sec: %ld, usec: %ld\n", start_tv.tv_sec, start_tv.tv_usec);

    // Loop
    int jump = PAGE_SIZE / sizeof(int);
    int *arr = malloc(numPages * jump * sizeof(int));
    for (int trial = 0; trial < numTrials; trial++) {
        for (int i = 0; i < numPages * jump; i += jump) arr[i] += 1;
    }
    free(arr);

    // End
    if (gettimeofday(&end_tv, NULL) == -1) errExit("gettimeofday failed!\n");
    printf("sec: %ld, usec: %ld\n", end_tv.tv_sec, end_tv.tv_usec);

    long timePerTrial = ((end_tv.tv_sec - start_tv.tv_sec) * 1000 
        + end_tv.tv_usec - start_tv.tv_usec) / numTrials;
    printf("Time cost per trial: %ld us\n", timePerTrial);
    
    return 0;
}
