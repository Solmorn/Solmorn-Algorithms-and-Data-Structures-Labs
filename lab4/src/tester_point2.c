#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/point2_hash_tables.h"
#include "../include/point2_testing.h"

#define POINT2_INSERT_BENCH_COUNT 1000000u
#define POINT2_OPS_MIN 10000u
#define POINT2_OPS_MAX 1000000u
#define POINT2_OPS_STEP 10000u

typedef enum Point2TableKind {
    POINT2_CHAIN_KIND = 0,
    POINT2_LINEAR_KIND = 1,
    POINT2_QUADRATIC_KIND = 2,
    POINT2_DOUBLE_KIND = 3,
    POINT2_CUCKOO_KIND = 4,
    POINT2_TABLE_KIND_COUNT = 5
} Point2TableKind;

static const char* point2_table_names[POINT2_TABLE_KIND_COUNT] = {
    "chaining",
    "linear_probing",
    "quadratic_probing",
    "double_hashing",
    "cuckoo"
};

static const double point2_load_factor_scan[] = {0.50, 0.60, 0.70, 0.80, 0.90};
#define POINT2_LOAD_FACTOR_SCAN_COUNT (sizeof(point2_load_factor_scan) / sizeof(point2_load_factor_scan[0]))

static double point2_default_load_factor(Point2TableKind kind) {
    switch (kind) {
        case POINT2_CHAIN_KIND:     return 1.00;
        case POINT2_LINEAR_KIND:    return 0.70;
        case POINT2_QUADRATIC_KIND: return 0.50;
        case POINT2_DOUBLE_KIND:    return 0.70;
        case POINT2_CUCKOO_KIND:    return 0.45;
        default:                    return 0.70;
    }
}

static void point2_write_load_factor_header(FILE* file) {
    fprintf(file, "table,load_factor,time_seconds\n");
}

static void point2_write_ops_header(FILE* file) {
    fprintf(file, "operations,table,time_seconds,final_size\n");
}

static double point2_benchmark_insert_chain(double max_load_factor) {
    Point2ChainTable table;
    point2_chain_init(&table, max_load_factor);

    srand(42);
    const double begin = point2_now_seconds();
    for (size_t i = 0; i < POINT2_INSERT_BENCH_COUNT; ++i) {
        (void)point2_chain_insert(&table, point2_rand_int());
    }
    const double end = point2_now_seconds();

    point2_chain_destroy(&table);
    return end - begin;
}

static double point2_benchmark_insert_probe(int mode, double max_load_factor) {
    Point2ProbeTable table;
    point2_probe_init(&table, mode, max_load_factor);

    srand(42);
    const double begin = point2_now_seconds();
    for (size_t i = 0; i < POINT2_INSERT_BENCH_COUNT; ++i) {
        (void)point2_probe_insert(&table, point2_rand_int());
    }
    const double end = point2_now_seconds();

    point2_probe_destroy(&table);
    return end - begin;
}

static double point2_benchmark_insert_cuckoo(double max_load_factor) {
    Point2CuckooTable table;
    point2_cuckoo_init(&table, max_load_factor);

    srand(42);
    const double begin = point2_now_seconds();
    for (size_t i = 0; i < POINT2_INSERT_BENCH_COUNT; ++i) {
        (void)point2_cuckoo_insert(&table, point2_rand_int());
    }
    const double end = point2_now_seconds();

    point2_cuckoo_destroy(&table);
    return end - begin;
}

static double point2_benchmark_insert(Point2TableKind kind, double max_load_factor) {
    switch (kind) {
        case POINT2_CHAIN_KIND:
            return point2_benchmark_insert_chain(max_load_factor);
        case POINT2_LINEAR_KIND:
            return point2_benchmark_insert_probe(POINT2_LINEAR_MODE, max_load_factor);
        case POINT2_QUADRATIC_KIND:
            return point2_benchmark_insert_probe(POINT2_QUADRATIC_MODE, max_load_factor);
        case POINT2_DOUBLE_KIND:
            return point2_benchmark_insert_probe(POINT2_DOUBLE_MODE, max_load_factor);
        case POINT2_CUCKOO_KIND:
            return point2_benchmark_insert_cuckoo(max_load_factor);
        default:
            return 0.0;
    }
}

static size_t point2_run_ops_chain(size_t operation_count, double max_load_factor, double p_insert, double p_find, double p_erase, unsigned seed) {
    Point2ChainTable table;
    point2_chain_init(&table, max_load_factor);
    srand(seed);

    for (size_t i = 0; i < operation_count; ++i) {
        const int op = point2_choose_operation(p_insert, p_find, p_erase);
        const int key = point2_rand_int();
        if (op == POINT2_INSERT_OP) {
            (void)point2_chain_insert(&table, key);
        } else if (op == POINT2_FIND_OP) {
            (void)point2_chain_find(&table, key);
        } else {
            (void)point2_chain_erase(&table, key);
        }
    }

    const size_t result = table.size;
    point2_chain_destroy(&table);
    return result;
}

static size_t point2_run_ops_probe(int mode, size_t operation_count, double max_load_factor, double p_insert, double p_find, double p_erase, unsigned seed) {
    Point2ProbeTable table;
    point2_probe_init(&table, mode, max_load_factor);
    srand(seed);

    for (size_t i = 0; i < operation_count; ++i) {
        const int op = point2_choose_operation(p_insert, p_find, p_erase);
        const int key = point2_rand_int();
        if (op == POINT2_INSERT_OP) {
            (void)point2_probe_insert(&table, key);
        } else if (op == POINT2_FIND_OP) {
            (void)point2_probe_contains(&table, key);
        } else {
            (void)point2_probe_erase(&table, key);
        }
    }

    const size_t result = table.size;
    point2_probe_destroy(&table);
    return result;
}

static size_t point2_run_ops_cuckoo(size_t operation_count, double max_load_factor, double p_insert, double p_find, double p_erase, unsigned seed) {
    Point2CuckooTable table;
    point2_cuckoo_init(&table, max_load_factor);
    srand(seed);

    for (size_t i = 0; i < operation_count; ++i) {
        const int op = point2_choose_operation(p_insert, p_find, p_erase);
        const int key = point2_rand_int();
        if (op == POINT2_INSERT_OP) {
            (void)point2_cuckoo_insert(&table, key);
        } else if (op == POINT2_FIND_OP) {
            (void)point2_cuckoo_contains(&table, key);
        } else {
            (void)point2_cuckoo_erase(&table, key);
        }
    }

    const size_t result = table.size;
    point2_cuckoo_destroy(&table);
    return result;
}

static double point2_benchmark_ops(Point2TableKind kind, size_t operation_count, double p_insert, double p_find, double p_erase, size_t* final_size) {
    const double load_factor = point2_default_load_factor(kind);
    const unsigned seed = 42u + (unsigned)operation_count * 13u + (unsigned)(p_insert * 100.0);

    const double begin = point2_now_seconds();
    switch (kind) {
        case POINT2_CHAIN_KIND:
            *final_size = point2_run_ops_chain(operation_count, load_factor, p_insert, p_find, p_erase, seed);
            break;
        case POINT2_LINEAR_KIND:
            *final_size = point2_run_ops_probe(POINT2_LINEAR_MODE, operation_count, load_factor, p_insert, p_find, p_erase, seed);
            break;
        case POINT2_QUADRATIC_KIND:
            *final_size = point2_run_ops_probe(POINT2_QUADRATIC_MODE, operation_count, load_factor, p_insert, p_find, p_erase, seed);
            break;
        case POINT2_DOUBLE_KIND:
            *final_size = point2_run_ops_probe(POINT2_DOUBLE_MODE, operation_count, load_factor, p_insert, p_find, p_erase, seed);
            break;
        case POINT2_CUCKOO_KIND:
            *final_size = point2_run_ops_cuckoo(operation_count, load_factor, p_insert, p_find, p_erase, seed);
            break;
        default:
            *final_size = 0u;
            break;
    }
    const double end = point2_now_seconds();
    return end - begin;
}

static void point2_run_load_factor_experiment(void) {
    FILE* file = fopen("./results/results_point2_load_factor.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "failed to open results_point2_load_factor.csv\n");
        exit(EXIT_FAILURE);
    }

    point2_write_load_factor_header(file);
    for (size_t t = 0; t < POINT2_TABLE_KIND_COUNT; ++t) {
        for (size_t i = 0; i < POINT2_LOAD_FACTOR_SCAN_COUNT; ++i) {
            const double lf = point2_load_factor_scan[i];
            const double seconds = point2_benchmark_insert((Point2TableKind)t, lf);
            fprintf(file, "%s,%.2f,%.9f\n", point2_table_names[t], lf, seconds);
            fflush(file);
        }
        printf("load-factor experiment done for %s\n", point2_table_names[t]);
    }

    fclose(file);
}

static void point2_run_ops_experiment(const char* path, double p_insert, double p_find, double p_erase) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "failed to open %s\n", path);
        exit(EXIT_FAILURE);
    }

    point2_write_ops_header(file);
    for (size_t n = POINT2_OPS_MIN; n <= POINT2_OPS_MAX; n += POINT2_OPS_STEP) {
        for (size_t t = 0; t < POINT2_TABLE_KIND_COUNT; ++t) {
            size_t final_size = 0u;
            const double seconds = point2_benchmark_ops((Point2TableKind)t, n, p_insert, p_find, p_erase, &final_size);
            fprintf(file, "%zu,%s,%.9f,%zu\n", n, point2_table_names[t], seconds, final_size);
        }
        fflush(file);
        printf("operations=%zu done for scenario p=(%.2f, %.2f, %.2f)\n", n, p_insert, p_find, p_erase);
    }

    fclose(file);
}

int main(void) {
    point2_run_load_factor_experiment();
    point2_run_ops_experiment("./results/results_point2_ops_equal.csv", 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);
    point2_run_ops_experiment("./results/results_point2_ops_bias.csv", 0.50, 0.25, 0.25);

    printf("Point 2 testing completed successfully.\n");
    return 0;
}

