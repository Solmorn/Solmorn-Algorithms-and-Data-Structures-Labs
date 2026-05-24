#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point9_sorts.h"

int main(void) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        "./tests/big_tests",
        lsd_radix_sort,
        "./results/results_lsd_radix.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/big_tests",
        msd_radix_sort,
        "./results/results_msd_radix.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    printf("Point 9 testing completed successfully.\n");
    return 0;
}