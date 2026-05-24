#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/avl_tree.h"
#include "../include/tree_testing.h"

int main(void) {
    const size_t begin_n = 100000;
    const size_t end_n = 1000000;
    const size_t step_n = 100000;
    const int run_count = 5;

    FILE* random_results = fopen("./results/results_point2_random.csv", "w");
    FILE* sorted_results = fopen("./results/results_point2_sorted.csv", "w");
    assert(random_results != NULL);
    assert(sorted_results != NULL);

    write_common_csv_header(random_results);
    write_common_csv_header(sorted_results);

    for (size_t n = begin_n; n <= end_n; n += step_n) {
        int* values = make_random_permutation(n, (unsigned int)n + 21u);
        BenchResult result = benchmark_tree(&AVL_TREE_VTABLE, values, n, n / 2, run_count);
        fprintf(random_results, "%zu,%.9f,%.9f\n", n, result.insert_time, result.erase_time);
        printf("Point 2 random n = %zu done\n", n);
        free(values);
    }

    int* sorted_values = make_sorted_array(end_n);
    BenchResult sorted_result = benchmark_tree(&AVL_TREE_VTABLE, sorted_values, end_n, end_n / 2, run_count);
    fprintf(sorted_results, "%zu,%.9f,%.9f\n", end_n, sorted_result.insert_time, sorted_result.erase_time);
    printf("Point 2 sorted n = %zu done\n", end_n);
    free(sorted_values);

    fclose(random_results);
    fclose(sorted_results);

    printf("Point 2 testing completed successfully.\n");

    return 0;
}
