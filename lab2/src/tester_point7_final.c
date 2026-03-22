#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point7_sorts.h"

int main(void) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        "./tests/big_tests",
        quick_sort_best_cutoff32,
        "./results/results_quick_best_cutoff32.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/big_tests",
        introsort_best,
        "./results/results_introsort_best.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    printf("Point 7 final comparison completed successfully.\n");
    return 0;
}