#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/point1_hashes.h"
#include "../include/point1_testing.h"

typedef uint64_t (*uint_hash_fn)(unsigned value);
typedef uint64_t (*float_hash_fn)(float value);
typedef uint64_t (*string_hash_fn)(const char* value);

typedef struct Point1SummaryRow {
    const char* hash_name;
    double time_seconds;
    double variance;
    uint32_t min_bucket;
    uint32_t max_bucket;
    double mean_bucket;
} Point1SummaryRow;

static void write_summary_header(FILE* file) {
    fprintf(file, "hash_name,time_seconds,variance,min_bucket,max_bucket,mean_bucket\n");
}

static void write_hist_header(FILE* file, const char* const* names, size_t name_count) {
    fprintf(file, "bucket");
    for (size_t i = 0; i < name_count; ++i) {
        fprintf(file, ",%s", names[i]);
    }
    fputc('\n', file);
}

static void write_hist_file(
    const char* path,
    const char* const* names,
    uint32_t (*counts)[POINT1_BUCKET_COUNT],
    size_t name_count
) {
    FILE* file = fopen(path, "w");
    assert(file != NULL);

    write_hist_header(file, names, name_count);
    for (size_t bucket = 0; bucket < POINT1_BUCKET_COUNT; ++bucket) {
        fprintf(file, "%zu", bucket);
        for (size_t i = 0; i < name_count; ++i) {
            fprintf(file, ",%u", counts[i][bucket]);
        }
        fputc('\n', file);
    }

    fclose(file);
}

static void write_summary_file(const char* path, const Point1SummaryRow* rows, size_t row_count) {
    FILE* file = fopen(path, "w");
    assert(file != NULL);

    write_summary_header(file);
    for (size_t i = 0; i < row_count; ++i) {
        fprintf(
            file,
            "%s,%.9f,%.9f,%u,%u,%.3f\n",
            rows[i].hash_name,
            rows[i].time_seconds,
            rows[i].variance,
            rows[i].min_bucket,
            rows[i].max_bucket,
            rows[i].mean_bucket
        );
    }

    fclose(file);
}

static void fill_buckets_uint(const unsigned* values, size_t n, uint_hash_fn fn, uint32_t* out_counts) {
    memset(out_counts, 0, POINT1_BUCKET_COUNT * sizeof(out_counts[0]));
    for (size_t i = 0; i < n; ++i) {
        const uint64_t hash = fn(values[i]);
        ++out_counts[hash % POINT1_BUCKET_COUNT];
    }
}

static double measure_uint_time(const unsigned* values, size_t n, uint_hash_fn fn, int run_count) {
    uint32_t counts[POINT1_BUCKET_COUNT];
    double total = 0.0;
    for (int run = 0; run < run_count; ++run) {
        const double begin = point1_now_seconds();
        fill_buckets_uint(values, n, fn, counts);
        const double end = point1_now_seconds();
        total += end - begin;
    }
    return total / (double)run_count;
}

static void fill_buckets_float(const float* values, size_t n, float_hash_fn fn, uint32_t* out_counts) {
    memset(out_counts, 0, POINT1_BUCKET_COUNT * sizeof(out_counts[0]));
    for (size_t i = 0; i < n; ++i) {
        const uint64_t hash = fn(values[i]);
        ++out_counts[hash % POINT1_BUCKET_COUNT];
    }
}

static double measure_float_time(const float* values, size_t n, float_hash_fn fn, int run_count) {
    uint32_t counts[POINT1_BUCKET_COUNT];
    double total = 0.0;
    for (int run = 0; run < run_count; ++run) {
        const double begin = point1_now_seconds();
        fill_buckets_float(values, n, fn, counts);
        const double end = point1_now_seconds();
        total += end - begin;
    }
    return total / (double)run_count;
}

static uint32_t g_crc32_table[256];

static uint64_t point1_hash_string_crc32_wrapper(const char* value) {
    return (uint64_t)point1_crc32_hash(value, g_crc32_table);
}

static void fill_buckets_string(const char* const* values, size_t n, string_hash_fn fn, uint32_t* out_counts) {
    memset(out_counts, 0, POINT1_BUCKET_COUNT * sizeof(out_counts[0]));
    for (size_t i = 0; i < n; ++i) {
        const uint64_t hash = fn(values[i]);
        ++out_counts[hash % POINT1_BUCKET_COUNT];
    }
}

static double measure_string_time(const char* const* values, size_t n, string_hash_fn fn, int run_count) {
    uint32_t counts[POINT1_BUCKET_COUNT];
    double total = 0.0;
    for (int run = 0; run < run_count; ++run) {
        const double begin = point1_now_seconds();
        fill_buckets_string(values, n, fn, counts);
        const double end = point1_now_seconds();
        total += end - begin;
    }
    return total / (double)run_count;
}

static void test_uints(void) {
    static const char* names[POINT1_UINT_HASH_COUNT] = {
        "remainder",
        "bit_repr",
        "knuth_mult"
    };
    static const uint_hash_fn fns[POINT1_UINT_HASH_COUNT] = {
        point1_hash_uint_remainder,
        point1_hash_uint_bit_repr,
        point1_hash_uint_knuth
    };

    size_t n = 0;
    unsigned* values = point1_load_uints("./tests/point1/uint_keys.txt", &n);

    uint32_t counts[POINT1_UINT_HASH_COUNT][POINT1_BUCKET_COUNT];
    Point1SummaryRow rows[POINT1_UINT_HASH_COUNT];

    for (size_t i = 0; i < POINT1_UINT_HASH_COUNT; ++i) {
        fill_buckets_uint(values, n, fns[i], counts[i]);
        rows[i].hash_name = names[i];
        rows[i].time_seconds = measure_uint_time(values, n, fns[i], 5);
        rows[i].variance = point1_variance_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].min_bucket = point1_min_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].max_bucket = point1_max_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].mean_bucket = (double)n / (double)POINT1_BUCKET_COUNT;
    }

    write_hist_file("./results/results_point1_uint_hist.csv", names, counts, POINT1_UINT_HASH_COUNT);
    write_summary_file("./results/results_point1_uint_summary.csv", rows, POINT1_UINT_HASH_COUNT);
    free(values);
}

static void test_floats(void) {
    static const char* names[POINT1_FLOAT_HASH_COUNT] = {
        "cast_int_bits",
        "bit_repr",
        "mantissa",
        "exponent",
        "mantissa_x_exponent"
    };
    static const float_hash_fn fns[POINT1_FLOAT_HASH_COUNT] = {
        point1_hash_float_cast_int_bits,
        point1_hash_float_bits,
        point1_hash_float_mantissa,
        point1_hash_float_exponent,
        point1_hash_float_mantissa_times_exponent
    };

    size_t n = 0;
    float* values = point1_load_floats("./tests/point1/float_keys.txt", &n);

    uint32_t counts[POINT1_FLOAT_HASH_COUNT][POINT1_BUCKET_COUNT];
    Point1SummaryRow rows[POINT1_FLOAT_HASH_COUNT];

    for (size_t i = 0; i < POINT1_FLOAT_HASH_COUNT; ++i) {
        fill_buckets_float(values, n, fns[i], counts[i]);
        rows[i].hash_name = names[i];
        rows[i].time_seconds = measure_float_time(values, n, fns[i], 5);
        rows[i].variance = point1_variance_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].min_bucket = point1_min_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].max_bucket = point1_max_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].mean_bucket = (double)n / (double)POINT1_BUCKET_COUNT;
    }

    write_hist_file("./results/results_point1_float_hist.csv", names, counts, POINT1_FLOAT_HASH_COUNT);
    write_summary_file("./results/results_point1_float_summary.csv", rows, POINT1_FLOAT_HASH_COUNT);
    free(values);
}

static void test_strings(void) {
    static const char* names[POINT1_STRING_HASH_COUNT] = {
        "length",
        "sum_letters",
        "polynomial",
        "crc32"
    };
    static const string_hash_fn fns[POINT1_STRING_HASH_COUNT] = {
        point1_hash_string_length,
        point1_hash_string_sum,
        point1_hash_string_polynomial,
        point1_hash_string_crc32_wrapper
    };

    size_t n = 0;
    char** values = point1_load_strings("./tests/point1/string_keys.txt", &n);

    uint32_t counts[POINT1_STRING_HASH_COUNT][POINT1_BUCKET_COUNT];
    Point1SummaryRow rows[POINT1_STRING_HASH_COUNT];

    for (size_t i = 0; i < POINT1_STRING_HASH_COUNT; ++i) {
        fill_buckets_string((const char* const*)values, n, fns[i], counts[i]);
        rows[i].hash_name = names[i];
        rows[i].time_seconds = measure_string_time((const char* const*)values, n, fns[i], 5);
        rows[i].variance = point1_variance_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].min_bucket = point1_min_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].max_bucket = point1_max_u32(counts[i], POINT1_BUCKET_COUNT);
        rows[i].mean_bucket = (double)n / (double)POINT1_BUCKET_COUNT;
    }

    write_hist_file("./results/results_point1_string_hist.csv", names, counts, POINT1_STRING_HASH_COUNT);
    write_summary_file("./results/results_point1_string_summary.csv", rows, POINT1_STRING_HASH_COUNT);
    point1_free_strings(values, n);
}

int main(void) {
    point1_crc32_init(g_crc32_table);

    test_uints();
    printf("uint hashes done\n");

    test_floats();
    printf("float hashes done\n");

    test_strings();
    printf("string hashes done\n");

    printf("Point 1 testing completed successfully.\n");
    return 0;
}
