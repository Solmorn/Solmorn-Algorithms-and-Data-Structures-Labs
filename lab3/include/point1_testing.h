#ifndef POINT1_TESTING_H
#define POINT1_TESTING_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int* read_array_from_file(const char* path, size_t* n) {
    FILE* file = fopen(path, "r");
    assert(file != NULL);

    assert(fscanf(file, "%zu", n) == 1);

    int* arr = (int*)calloc(*n, sizeof(int));
    assert(arr != NULL);

    for (size_t i = 0; i < *n; ++i) {
        assert(fscanf(file, "%d", &arr[i]) == 1);
    }

    fclose(file);
    return arr;
}

static double current_time_seconds(void) {
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

static void write_csv_header(FILE* file) {
    fprintf(file, "n,linear_build,insert_build\n");
}

#endif
