#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point4_sorts.h"

int main(void) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        "./tests/big_tests",
        quick_sort_fat,
        "./results/results_quick_fat_basic.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/big_tests",
        quick_sort_fat_one_branch,
        "./results/results_quick_fat_one_branch.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/big_tests",
        quick_sort_fat_one_branch_cutoff40,
        "./results/results_quick_fat_one_branch_cutoff40.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    printf("Point 4 optimization comparison completed successfully.\n");
    return 0;
}

