#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"

static int int_cmp(const void* lhs, const void* rhs) {
    int a = *(const int*)lhs;
    int b = *(const int*)rhs;

    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

static void qsort_wrapper_sort(int* arr, size_t n) {
    qsort(arr, n, sizeof(int), int_cmp);
}

static int parse_int(const char* s) {
    char* end = NULL;
    long value = strtol(s, &end, 10);
    assert(end != s && *end == '\0');
    return (int)value;
}

int main(int argc, char** argv) {
    assert(argc == 6);

    const char* tests_dir = argv[1];
    const char* results_path = argv[2];
    int from = parse_int(argv[3]);
    int to = parse_int(argv[4]);
    int step = parse_int(argv[5]);

    size_t result_count = 0;
    double* times = test_sorting(
        tests_dir,
        qsort_wrapper_sort,
        results_path,
        from,
        to,
        step,
        &result_count
    );

    free(times);
    return 0;
}