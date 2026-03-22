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

static void libc_qsort_sort(int* arr, size_t n) {
    qsort(arr, n, sizeof(int), int_cmp);
}

int main(void) {
    size_t count = 0;
    double* times = test_sorting(
        "./tests/big_tests",
        libc_qsort_sort,
        "./results/results_qsort_big.csv",
        0,
        1000000,
        10000,
        &count
    );

    free(times);

    printf("Point 10 qsort benchmarking completed successfully.\n");
    return 0;
}