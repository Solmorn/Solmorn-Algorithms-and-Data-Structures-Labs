#ifndef TESTING_H
#define TESTING_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void (*sort_fn_t)(int* arr, size_t n);

static void read_array_from_file(const char* path, int** arr, size_t* n) {
    FILE* file = fopen(path, "r");
    assert(file != NULL);

    assert(fscanf(file, "%zu", n) == 1);

    *arr = NULL;
    if (*n > 0) {
        *arr = (int*)malloc((*n) * sizeof(int));
        assert(*arr != NULL);
    }

    for (size_t i = 0; i < *n; ++i) {
        assert(fscanf(file, "%d", &(*arr)[i]) == 1);
    }

    fclose(file);
}

static double measure_sort_time(sort_fn_t sort_fn, int* arr, size_t n) {
    clock_t start = clock();
    sort_fn(arr, n);
    clock_t finish = clock();

    return (double)(finish - start) / (double)CLOCKS_PER_SEC;
}

static double* test_sorting(
    const char* tests_dir,
    sort_fn_t sort_fn,
    const char* results_path,
    int from,
    int to,
    int step,
    size_t* result_count
) {
    assert(tests_dir != NULL);
    assert(sort_fn != NULL);
    assert(results_path != NULL);
    assert(step > 0);
    assert(to >= from);
    assert(result_count != NULL);

    size_t count = (size_t)((to - from) / step + 1);
    double* avg_times = (double*)calloc(count, sizeof(double));
    assert(avg_times != NULL);

    FILE* results = fopen(results_path, "w");
    assert(results != NULL);

    fprintf(results, "size,avg_time_sec\n");

    size_t index = 0;

    for (int size = from; size <= to; size += step, ++index) {
        double total_time = 0.0;
        size_t tests_for_size = 0;

        for (size_t k = 0;; ++k) {
            char in_path[4096];
            char out_path[4096];

            snprintf(in_path, sizeof(in_path), "%s/%d_%zu.in", tests_dir, size, k);
            snprintf(out_path, sizeof(out_path), "%s/%d_%zu.out", tests_dir, size, k);

            FILE* probe = fopen(in_path, "r");
            if (probe == NULL) {
                break;
            }
            fclose(probe);

            int* input_arr = NULL;
            int* expected_arr = NULL;
            int* work_arr = NULL;
            size_t input_n = 0;
            size_t expected_n = 0;

            read_array_from_file(in_path, &input_arr, &input_n);
            read_array_from_file(out_path, &expected_arr, &expected_n);

            assert(input_n == expected_n);
            assert((int)input_n == size);

            if (input_n > 0) {
                work_arr = (int*)malloc(input_n * sizeof(int));
                assert(work_arr != NULL);
                memcpy(work_arr, input_arr, input_n * sizeof(int));
            }

            total_time += measure_sort_time(sort_fn, work_arr, input_n);

            if (input_n > 0) {
                assert(memcmp(work_arr, expected_arr, input_n * sizeof(int)) == 0);
            }

            free(input_arr);
            free(expected_arr);
            free(work_arr);

            ++tests_for_size;
        }

        assert(tests_for_size > 0);

        avg_times[index] = total_time / (double)tests_for_size;
        fprintf(results, "%d,%.9f\n", size, avg_times[index]);
    }

    fclose(results);
    *result_count = count;
    return avg_times;
}

#endif