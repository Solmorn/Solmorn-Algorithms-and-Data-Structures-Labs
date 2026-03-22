#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point7_sorts.h"

typedef struct {
    const char* name;
    sort_fn_t sort_fn;
    const char* result_path;
} sort_case_t;

int main(void) {
    sort_case_t cases[] = {
        {"introsort C=1", introsort_c1, "./results/results_introsort_c1.csv"},
        {"introsort C=2", introsort_c2, "./results/results_introsort_c2.csv"},
        {"introsort C=3", introsort_c3, "./results/results_introsort_c3.csv"},
        {"introsort C=4", introsort_c4, "./results/results_introsort_c4.csv"}
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

    printf("Point 7 C-comparison completed successfully.\n");
    return 0;
}

