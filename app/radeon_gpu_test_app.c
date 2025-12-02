// radeon_gpu_test_app.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
    printf("Usage: radeon_gpu_test_app <options>\n");
    printf("Options:  \n");
    printf("  -h, --help       Show this help message\n");
    printf("  -m, --memory     Specify memory size for testing (in MB)\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return EXIT_FAILURE;
    }

    size_t memory_size = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage();
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--memory") == 0) {
            if (i + 1 < argc) {
                memory_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: Memory size not specified.\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (memory_size == 0) {
        fprintf(stderr, "Error: No memory size specified.\n");
        return EXIT_FAILURE;
    }

    printf("Testing GPU memory: %zu MB\n", memory_size);
    // Add GPU memory allocation and testing logic here.

    return EXIT_SUCCESS;
}