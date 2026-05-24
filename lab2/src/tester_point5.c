#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point5_sorts.h"

typedef struct {
    const char* name;
    sort_fn_t sort_fn;
    const char* result_path;
} sort_case_t;

int main(void) {
    sort_case_t cases[] = {
        {"pivot middle",            quick_sort_pivot_middle,            "./results/results_pivot_middle.csv"},
        {"pivot median3",           quick_sort_pivot_median3,           "./results/results_pivot_median3.csv"},
        {"pivot random",            quick_sort_pivot_random,            "./results/results_pivot_random.csv"},
        {"pivot median3 random",    quick_sort_pivot_median3_random,    "./results/results_pivot_median3_random.csv"},
        {"pivot median of medians", quick_sort_pivot_median_of_medians, "./results/results_pivot_median_of_medians.csv"}
    };

    size_t case_count = sizeof(cases) / sizeof(cases[0]);

    for (size_t i = 0; i < case_count; ++i) {
        size_t result_count = 0;
        double* times = test_sorting(
            "./tests/big_tests",
            cases[i].sort_fn,
            cases[i].result_path,
            0,
            1000000,
            10000,
            &result_count
        );

        free(times);
        printf("%s completed\n", cases[i].name);
    }

    printf("Point 5 testing completed successfully.\n");
    return 0;
}