#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/point1_heaps.h"
#include "../include/point1_testing.h"

static void fill_heap_data(const int* src, HeapElem* dst, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        dst[i].value = src[i];
        dst[i].request_index = (int)i;
    }
}

int main(void) {
    const size_t begin_n = 100000;
    const size_t end_n = 1000000;
    const size_t step_n = 100000;
    const int run_count = 5;

    FILE* results = fopen("./results/results_point1.csv", "w");
    assert(results != NULL);

    write_csv_header(results);

    for (size_t n = begin_n; n <= end_n; n += step_n) {
        char path[256];
        snprintf(path, sizeof(path), "./tests/point1/test_%zu.txt", n);

        size_t real_n = 0;
        int* source = read_array_from_file(path, &real_n);
        assert(real_n == n);

        double linear_time_sum = 0.0;
        double insert_time_sum = 0.0;

        for (int run = 0; run < run_count; ++run) {
            HeapElem* linear_data = (HeapElem*)calloc(n, sizeof(HeapElem));
            HeapElem* insert_data = (HeapElem*)calloc(n, sizeof(HeapElem));
            assert(linear_data != NULL);
            assert(insert_data != NULL);

            fill_heap_data(source, linear_data, n);
            fill_heap_data(source, insert_data, n);

            Heap* linear_heap = heap_view_ctor(linear_data, n);
            Heap* insert_heap = heap_view_ctor(insert_data, n);

            double start_linear = current_time_seconds();
            heap_build_linear(linear_heap);
            double end_linear = current_time_seconds();

            double start_insert = current_time_seconds();
            heap_build_by_inserts(insert_heap);
            double end_insert = current_time_seconds();

            assert(heap_is_valid(linear_heap));
            assert(heap_is_valid(insert_heap));
            assert(heap_get_min(linear_heap) == heap_get_min(insert_heap));

            linear_time_sum += end_linear - start_linear;
            insert_time_sum += end_insert - start_insert;

            heap_view_dtor(linear_heap);
            heap_view_dtor(insert_heap);
            free(linear_data);
            free(insert_data);
        }

        fprintf(
            results,
            "%zu,%.9f,%.9f\n",
            n,
            linear_time_sum / run_count,
            insert_time_sum / run_count
        );

        printf("n = %zu done\n", n);

        free(source);
    }

    fclose(results);

    printf("Point 1 testing completed successfully.\n");
    return 0;
}
