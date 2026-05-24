#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point6_sorts.h"

typedef struct {
    const char* name;
    sort_fn_t sort_fn;
    const char* result_path;
} sort_case_t;

int main(void) {
    sort_case_t cases[] = {
        {"shell cutoff t=8",  quick_sort_shell_cutoff_t8,  "./results/results_shell_cutoff_t8.csv"},
        {"shell cutoff t=16", quick_sort_shell_cutoff_t16, "./results/results_shell_cutoff_t16.csv"},
        {"shell cutoff t=24", quick_sort_shell_cutoff_t24, "./results/results_shell_cutoff_t24.csv"},
        {"shell cutoff t=32", quick_sort_shell_cutoff_t32, "./results/results_shell_cutoff_t32.csv"},
        {"shell cutoff t=40", quick_sort_shell_cutoff_t40, "./results/results_shell_cutoff_t40.csv"},
        {"shell cutoff t=48", quick_sort_shell_cutoff_t48, "./results/results_shell_cutoff_t48.csv"},
        {"shell cutoff t=64", quick_sort_shell_cutoff_t64, "./results/results_shell_cutoff_t64.csv"}
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

    printf("Point 6 testing completed successfully.\n");
    return 0;
}