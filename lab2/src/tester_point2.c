#include <stdio.h>
#include <stdlib.h>

#include "../include/testing.h"
#include "../include/point2_sorts.h"

typedef struct {
    const char* name;
    sort_fn_t sort_fn;
    const char* result_path;
} sort_case_t;

int main(void) {
    sort_case_t cases[] = {
        {"heap k=2",  heap_sort_k2,  "./results/results_heap_k2.csv"},
        {"heap k=3",  heap_sort_k3,  "./results/results_heap_k3.csv"},
        {"heap k=4",  heap_sort_k4,  "./results/results_heap_k4.csv"},
        {"heap k=5",  heap_sort_k5,  "./results/results_heap_k5.csv"},
        {"heap k=6",  heap_sort_k6,  "./results/results_heap_k6.csv"},
        {"heap k=7",  heap_sort_k7,  "./results/results_heap_k7.csv"},
        {"heap k=8",  heap_sort_k8,  "./results/results_heap_k8.csv"},
        {"heap k=9",  heap_sort_k9,  "./results/results_heap_k9.csv"},
        {"heap k=10", heap_sort_k10, "./results/results_heap_k10.csv"}
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

    printf("Point 2 testing completed successfully.\n");
    return 0;
}