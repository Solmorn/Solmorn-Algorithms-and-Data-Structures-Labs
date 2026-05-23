#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/point2_binomial_heap.h"
#include "../include/point2_testing.h"

static long long array_min(const int* arr, size_t n) {
    assert(arr != NULL);
    assert(n > 0);

    long long min_value = arr[0];

    for (size_t i = 1; i < n; ++i) {
        if ((long long)arr[i] < min_value) {
            min_value = arr[i];
        }
    }

    return min_value;
}

int main(void) {
    const size_t begin_n = 100000;
    const size_t end_n = 1000000;
    const size_t step_n = 100000;
    const int run_count = 5;

    FILE* results = fopen("./results/results_point2.csv", "w");
    assert(results != NULL);

    write_csv_header(results);

    for (size_t n = begin_n; n <= end_n; n += step_n) {
        char path[256];
        snprintf(path, sizeof(path), "./tests/point2/test_%zu.txt", n);

        size_t real_n = 0;
        int* source = read_array_from_file(path, &real_n);
        assert(real_n == n);

        long long expected_min = array_min(source, n);
        double build_time_sum = 0.0;

        for (int run = 0; run < run_count; ++run) {
            Heap* heap = heap_ctor();

            double start = current_time_seconds();

            for (size_t i = 0; i < n; ++i) {
                heap_insert(heap, source[i], (int)i + 1);
            }

            double end = current_time_seconds();

            assert(heap->head != NULL);
            assert(heap_is_valid(heap));
            assert(heap_get_min(heap) == expected_min);

            build_time_sum += end - start;

            heap_dtor(heap);
        }

        fprintf(results, "%zu,%.9f\n", n, build_time_sum / run_count);
        printf("n = %zu done\n", n);

        free(source);
    }

    fclose(results);

    printf("Point 2 testing completed successfully.\n");
    return 0;
}
