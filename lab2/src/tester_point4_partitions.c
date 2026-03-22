#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point4_sorts.h"

static void run_suite(
    const char* tests_dir,
    const char* lomuto_csv,
    const char* hoare_csv,
    const char* fat_csv
) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        tests_dir,
        quick_sort_lomuto,
        lomuto_csv,
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        tests_dir,
        quick_sort_hoare,
        hoare_csv,
        0,
        1000000,
        10000,
        &count
    );
    free(times);

    times = test_sorting(
        tests_dir,
        quick_sort_fat,
        fat_csv,
        0,
        1000000,
        10000,
        &count
    );
    free(times);
}

int main(void) {
    run_suite(
        "./tests/big_tests",
        "./results/results_quick_lomuto_big.csv",
        "./results/results_quick_hoare_big.csv",
        "./results/results_quick_fat_big.csv"
    );

    run_suite(
        "./tests/test_most_dublicates",
        "./results/results_quick_lomuto_duplicates.csv",
        "./results/results_quick_hoare_duplicates.csv",
        "./results/results_quick_fat_duplicates.csv"
    );

    printf("Point 4 partition comparison completed successfully.\n");
    return 0;
}