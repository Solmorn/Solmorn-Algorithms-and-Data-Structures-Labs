#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/naive_tree.h"
#include "../include/tree_testing.h"

int main(void) {
    const int run_count = 5;
    const size_t random_n = 100000;
    const size_t sorted_n = 10000;

    FILE* results = fopen("./results/results_point1.csv", "w");
    assert(results != NULL);

    fprintf(results, "type,n,insert_time,erase_time\n");

    int* random_values = make_random_permutation(random_n, 11u);
    BenchResult random_result = benchmark_tree(&NAIVE_TREE_VTABLE, random_values, random_n, random_n / 2, run_count);
    fprintf(results, "random,%zu,%.9f,%.9f\n", random_n, random_result.insert_time, random_result.erase_time);
    printf("Point 1 random n = %zu done\n", random_n);
    free(random_values);

    int* sorted_values = make_sorted_array(sorted_n);
    BenchResult sorted_result = benchmark_tree(&NAIVE_TREE_VTABLE, sorted_values, sorted_n, sorted_n / 2, run_count);
    fprintf(results, "sorted,%zu,%.9f,%.9f\n", sorted_n, sorted_result.insert_time, sorted_result.erase_time);
    printf("Point 1 sorted n = %zu done\n", sorted_n);
    free(sorted_values);

    fclose(results);

    printf("Point 1 testing completed successfully.\n");

    return 0;
}
