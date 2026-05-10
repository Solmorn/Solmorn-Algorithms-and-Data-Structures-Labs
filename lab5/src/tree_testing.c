#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../include/tree_testing.h"

static unsigned int next_random(unsigned int* state) {
    assert(state != NULL);

    unsigned int x = *state;
    x ^= x << 13u;
    x ^= x >> 17u;
    x ^= x << 5u;
    *state = x;

    return x;
}

double current_time_seconds(void) {
    struct timeval time = {0};

    gettimeofday(&time, NULL);

    return (double)time.tv_sec + (double)time.tv_usec / 1000000.0;
}

void shuffle_array(int* array, size_t size, unsigned int seed) {
    assert(array != NULL);

    unsigned int state = seed;

    for (size_t i = size; i > 1; i--) {
        size_t pos = (size_t)(next_random(&state) % (unsigned int)i);
        int tmp = array[i - 1];
        array[i - 1] = array[pos];
        array[pos] = tmp;
    }
}

int* make_random_permutation(size_t size, unsigned int seed) {
    int* array = (int*)calloc(size, sizeof(int));
    assert(array != NULL);

    for (size_t i = 0; i < size; i++) {
        array[i] = (int)i;
    }

    shuffle_array(array, size, seed);

    return array;
}

int* make_sorted_array(size_t size) {
    int* array = (int*)calloc(size, sizeof(int));
    assert(array != NULL);

    for (size_t i = 0; i < size; i++) {
        array[i] = (int)i;
    }

    return array;
}

BenchResult benchmark_tree(const TreeVTable* table, const int* values, size_t insert_cnt, size_t erase_cnt, int run_count) {
    assert(table != NULL);
    assert(values != NULL);
    assert(table->ctor != NULL);
    assert(table->dtor != NULL);
    assert(table->insert != NULL);
    assert(table->erase != NULL);
    assert(table->contains != NULL);
    assert(erase_cnt <= insert_cnt);
    assert(run_count > 0);

    BenchResult result = {0};

    for (int run = 0; run < run_count; run++) {
        void* tree = table->ctor();
        assert(tree != NULL);

        double start_insert = current_time_seconds();
        for (size_t i = 0; i < insert_cnt; i++) {
            bool inserted = table->insert(tree, values[i]);
            assert(inserted);
        }
        double end_insert = current_time_seconds();

        double start_erase = current_time_seconds();
        for (size_t i = 0; i < erase_cnt; i++) {
            bool erased = table->erase(tree, values[i]);
            assert(erased);
        }
        double end_erase = current_time_seconds();

        result.insert_time += end_insert - start_insert;
        result.erase_time += end_erase - start_erase;

        table->dtor(tree);
    }

    result.insert_time /= (double)run_count;
    result.erase_time /= (double)run_count;

    return result;
}

void write_common_csv_header(FILE* file) {
    assert(file != NULL);

    fprintf(file, "n,insert_time,erase_time\n");
}
