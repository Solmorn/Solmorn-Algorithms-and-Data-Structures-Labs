#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point3_testing.h"

static void run_suite(const char* mode, const char* csv_path) {
    FILE* results = fopen(csv_path, "w");
    assert(results != NULL);

    write_csv_header(results);

    for (int n = 1000; n <= 5000; n += 1000) {
        char path[256];
        snprintf(path, sizeof(path), "./tests/point3/%s_%d.bin", mode, n);

        Graph* graph = graph_read_binary(path);

        long long* dist_naive = (long long*)calloc((size_t)graph->n, sizeof(long long));
        long long* dist_binary = (long long*)calloc((size_t)graph->n, sizeof(long long));
        long long* dist_binomial = (long long*)calloc((size_t)graph->n, sizeof(long long));
        long long* dist_fibonacci = (long long*)calloc((size_t)graph->n, sizeof(long long));

        assert(dist_naive != NULL);
        assert(dist_binary != NULL);
        assert(dist_binomial != NULL);
        assert(dist_fibonacci != NULL);

        double start = current_time_seconds();
        dijkstra_naive(graph, 0, dist_naive);
        double naive_time = current_time_seconds() - start;

        start = current_time_seconds();
        dijkstra_binary(graph, 0, dist_binary);
        double binary_time = current_time_seconds() - start;

        start = current_time_seconds();
        dijkstra_binomial(graph, 0, dist_binomial);
        double binomial_time = current_time_seconds() - start;

        start = current_time_seconds();
        dijkstra_fibonacci(graph, 0, dist_fibonacci);
        double fibonacci_time = current_time_seconds() - start;

        assert(distances_equal(dist_naive, dist_binary, graph->n));
        assert(distances_equal(dist_naive, dist_binomial, graph->n));
        assert(distances_equal(dist_naive, dist_fibonacci, graph->n));

        fprintf(
            results,
            "%d,%.9f,%.9f,%.9f,%.9f\n",
            n,
            naive_time,
            binary_time,
            binomial_time,
            fibonacci_time
        );

        printf("%s n = %d done\n", mode, n);

        free(dist_naive);
        free(dist_binary);
        free(dist_binomial);
        free(dist_fibonacci);
        graph_dtor(graph);
    }

    fclose(results);
}

int main(void) {
    run_suite("sparse", "./results/results_point3_sparse.csv");
    run_suite("dense", "./results/results_point3_dense.csv");

    printf("Point 3 testing completed successfully.\n");
    return 0;
}
