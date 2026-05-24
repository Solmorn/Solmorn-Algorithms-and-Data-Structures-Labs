#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point3_sorts.h"

int main(void) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        "./tests/big_tests",
        merge_sort_recursive,
        "./results/results_merge_recursive.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/big_tests",
        merge_sort_iterative,
        "./results/results_merge_iterative.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    printf("Point 3 testing completed successfully.\n");
    return 0;
}