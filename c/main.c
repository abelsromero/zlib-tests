#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "decompress.h"

void print_usage(const char *program_name) {
    printf("Usage: %s <compress|decompress> <input_file> <output_file>\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *operation = argv[1];
    const char *input_file = argv[2];
    const char *output_file = argv[3];

    if (strcmp(operation, "compress") == 0) {
        if (compress_file(input_file, output_file) != 0) {
            fprintf(stderr, "Error compressing file.\n");
            return 1;
        }
    } else if (strcmp(operation, "decompress") == 0) {
        if (decompress_file(input_file, output_file) != 0) {
            fprintf(stderr, "Error decompressing file.\n");
            return 1;
        }
    } else {
        print_usage(argv[0]);
        return 1;
    }

    printf("Operation '%s' completed successfully.\n", operation);
    return 0;
}