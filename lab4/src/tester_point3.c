#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/point2_hash_tables.h"
#include "../include/point3_perfect_hash.h"
#include "../include/point3_testing.h"

typedef struct Point3SummaryRow {
    const char* table_name;
    double build_time_seconds;
    double search_time_seconds;
    size_t found_count;
} Point3SummaryRow;

static void point3_write_summary(const Point3SummaryRow* rows, size_t row_count) {
    FILE* file = fopen("./results/results_point3_summary.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "failed to open results_point3_summary.csv\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "table,build_time_seconds,search_time_seconds,queries,found_count\n");
    for (size_t i = 0; i < row_count; ++i) {
        fprintf(
            file,
            "%s,%.9f,%.9f,%u,%zu\n",
            rows[i].table_name,
            rows[i].build_time_seconds,
            rows[i].search_time_seconds,
            (unsigned)POINT3_QUERY_COUNT,
            rows[i].found_count
        );
    }

    fclose(file);
}

static Point3SummaryRow point3_benchmark_chaining(const int* keys, const int* queries) {
    Point3SummaryRow row = {"chaining", 0.0, 0.0, 0u};
    Point2ChainTable table;

    const double build_begin = point3_now_seconds();
    point2_chain_init(&table, 1.00);
    for (size_t i = 0; i < POINT3_KEY_COUNT; ++i) {
        (void)point2_chain_insert(&table, keys[i]);
    }
    const double build_end = point3_now_seconds();

    size_t found = 0u;
    const double search_begin = point3_now_seconds();
    for (size_t i = 0; i < POINT3_QUERY_COUNT; ++i) {
        found += point2_chain_find(&table, queries[i]) ? 1u : 0u;
    }
    const double search_end = point3_now_seconds();

    row.build_time_seconds = build_end - build_begin;
    row.search_time_seconds = search_end - search_begin;
    row.found_count = found;

    point2_chain_destroy(&table);
    return row;
}

static Point3SummaryRow point3_benchmark_double_hashing(const int* keys, const int* queries) {
    Point3SummaryRow row = {"double_hashing", 0.0, 0.0, 0u};
    Point2ProbeTable table;

    const double build_begin = point3_now_seconds();
    point2_probe_init(&table, POINT2_DOUBLE_MODE, 0.70);
    for (size_t i = 0; i < POINT3_KEY_COUNT; ++i) {
        (void)point2_probe_insert(&table, keys[i]);
    }
    const double build_end = point3_now_seconds();

    size_t found = 0u;
    const double search_begin = point3_now_seconds();
    for (size_t i = 0; i < POINT3_QUERY_COUNT; ++i) {
        found += point2_probe_contains(&table, queries[i]) ? 1u : 0u;
    }
    const double search_end = point3_now_seconds();

    row.build_time_seconds = build_end - build_begin;
    row.search_time_seconds = search_end - search_begin;
    row.found_count = found;

    point2_probe_destroy(&table);
    return row;
}

static Point3SummaryRow point3_benchmark_cuckoo(const int* keys, const int* queries) {
    Point3SummaryRow row = {"cuckoo", 0.0, 0.0, 0u};
    Point2CuckooTable table;

    const double build_begin = point3_now_seconds();
    point2_cuckoo_init(&table, 0.45);
    for (size_t i = 0; i < POINT3_KEY_COUNT; ++i) {
        (void)point2_cuckoo_insert(&table, keys[i]);
    }
    const double build_end = point3_now_seconds();

    size_t found = 0u;
    const double search_begin = point3_now_seconds();
    for (size_t i = 0; i < POINT3_QUERY_COUNT; ++i) {
        found += point2_cuckoo_contains(&table, queries[i]) ? 1u : 0u;
    }
    const double search_end = point3_now_seconds();

    row.build_time_seconds = build_end - build_begin;
    row.search_time_seconds = search_end - search_begin;
    row.found_count = found;

    point2_cuckoo_destroy(&table);
    return row;
}

static Point3SummaryRow point3_benchmark_perfect_hash(const int* keys, const int* queries) {
    Point3SummaryRow row = {"perfect_hashing", 0.0, 0.0, 0u};
    Point3PerfectHash table;
    point3_perfect_hash_init_empty(&table);

    const double build_begin = point3_now_seconds();
    if (!point3_perfect_hash_build(&table, keys, POINT3_KEY_COUNT)) {
        fprintf(stderr, "failed to build perfect hash\n");
        exit(EXIT_FAILURE);
    }
    const double build_end = point3_now_seconds();

    size_t found = 0u;
    const double search_begin = point3_now_seconds();
    for (size_t i = 0; i < POINT3_QUERY_COUNT; ++i) {
        found += point3_perfect_hash_contains(&table, queries[i]) ? 1u : 0u;
    }
    const double search_end = point3_now_seconds();

    row.build_time_seconds = build_end - build_begin;
    row.search_time_seconds = search_end - search_begin;
    row.found_count = found;

    point3_perfect_hash_destroy(&table);
    return row;
}

int main(void) {
    int* keys = (int*)malloc(POINT3_KEY_COUNT * sizeof(keys[0]));
    int* queries = (int*)malloc(POINT3_QUERY_COUNT * sizeof(queries[0]));

    if (keys == NULL || queries == NULL) {
        fprintf(stderr, "allocation failed\n");
        free(keys);
        free(queries);
        return EXIT_FAILURE;
    }

    srand(42);
    (void)point3_generate_distinct_keys(keys, POINT3_KEY_COUNT);
    (void)point3_generate_queries(keys, POINT3_KEY_COUNT, queries, POINT3_QUERY_COUNT);

    Point3SummaryRow rows[4];
    rows[0] = point3_benchmark_chaining(keys, queries);
    printf("done: %s\n", rows[0].table_name);

    rows[1] = point3_benchmark_double_hashing(keys, queries);
    printf("done: %s\n", rows[1].table_name);

    rows[2] = point3_benchmark_cuckoo(keys, queries);
    printf("done: %s\n", rows[2].table_name);

    rows[3] = point3_benchmark_perfect_hash(keys, queries);
    printf("done: %s\n", rows[3].table_name);

    point3_write_summary(rows, 4u);

    free(keys);
    free(queries);

    printf("Point 3 testing completed successfully.\n");
    return 0;
}