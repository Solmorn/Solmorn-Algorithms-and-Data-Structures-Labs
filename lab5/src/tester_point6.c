#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/b_tree.h"
#include "../include/tree_testing.h"

int main(void) {
    const size_t begin_n = 100000;
    const size_t end_n = 1000000;
    const size_t step_n = 100000;
    const int run_count = 5;

    FILE* results = fopen("./results/results_point6.csv", "w");
    assert(results != NULL);

    write_common_csv_header(results);

    for (size_t n = begin_n; n <= end_n; n += step_n) {
        int* values = make_random_permutation(n, (unsigned int)n + 61u);
        BenchResult result = benchmark_tree(&B_TREE_VTABLE, values, n, n / 2, run_count);
        fprintf(results, "%zu,%.9f,%.9f\n", n, result.insert_time, result.erase_time);
        printf("Point 6 n = %zu done\n", n);
        free(values);
    }

    fclose(results);

    printf("Point 6 testing completed successfully.\n");

    return 0;
}
