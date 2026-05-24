#include <cstdio>
#include <cstdlib>

#include "../include/testing.h"
#include "../include/point8_sorts.h"

int main(void) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        "./tests/big_tests",
        timsort_github,
        "./results/results_timsort_github.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/big_tests",
        pdqsort_github,
        "./results/results_pdqsort_github.csv",
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    std::printf("Point 8 testing completed successfully.\n");
    return 0;
}