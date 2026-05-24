#ifndef TREE_TESTING_H
#define TREE_TESTING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct TreeVTable {
    void* (*ctor)(void);
    void (*dtor)(void* tree);
    bool (*insert)(void* tree, int key);
    bool (*erase)(void* tree, int key);
    bool (*contains)(void* tree, int key);
} TreeVTable;

typedef struct BenchResult {
    double insert_time;
    double erase_time;
} BenchResult;

double current_time_seconds(void);
void shuffle_array(int* array, size_t size, unsigned int seed);
int* make_random_permutation(size_t size, unsigned int seed);
int* make_sorted_array(size_t size);
BenchResult benchmark_tree(const TreeVTable* table, const int* values, size_t insert_cnt, size_t erase_cnt, int run_count);
void write_common_csv_header(FILE* file);

#endif
