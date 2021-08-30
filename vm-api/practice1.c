#include <stddef.h>

int main(int argc, char const *argv[])
{
    int *ptr = NULL;
    *ptr += 1; // Segmentation fault
    return 0;
}
