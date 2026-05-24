#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point1_sorts.h"

int main(void) {
    size_t count = 0;
    double* times = NULL;

    times = test_sorting(
        "./tests/small_tests",
        insertion_sort,
        "./results/results_insertion.csv",
        0,
        1000,
        50,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/small_tests",
        bubble_sort,
        "./results/results_bubble.csv",
        0,
        1000,
        50,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/small_tests",
        selection_sort,
        "./results/results_selection.csv",
        0,
        1000,
        50,
        &count
    );
    free(times);

    times = test_sorting(
        "./tests/small_tests",
        shell_sort,
        "./results/results_shell.csv",
        0,
        1000,
        50,
        &count
    );
    free(times);

    printf("Point 1 testing completed successfully.\n");
    return 0;
}


