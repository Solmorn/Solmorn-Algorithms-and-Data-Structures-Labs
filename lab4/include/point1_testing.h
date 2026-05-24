#ifndef POINT1_TESTING_H
#define POINT1_TESTING_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static inline double point1_now_seconds(void) {
    struct timespec ts;
    const int rc = clock_gettime(CLOCK_MONOTONIC, &ts);
    assert(rc == 0);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static inline size_t point1_read_count(FILE* file) {
    size_t n = 0;
    const int rc = fscanf(file, "%zu", &n);
    assert(rc == 1);
    return n;
}

static inline unsigned* point1_load_uints(const char* path, size_t* out_n) {
    FILE* file = fopen(path, "r");
    assert(file != NULL);

    const size_t n = point1_read_count(file);
    unsigned* values = (unsigned*)malloc(n * sizeof(unsigned));
    assert(values != NULL);

    for (size_t i = 0; i < n; ++i) {
        const int rc = fscanf(file, "%u", &values[i]);
        assert(rc == 1);
    }

    fclose(file);
    *out_n = n;
    return values;
}

static inline float* point1_load_floats(const char* path, size_t* out_n) {
    FILE* file = fopen(path, "r");
    assert(file != NULL);

    const size_t n = point1_read_count(file);
    float* values = (float*)malloc(n * sizeof(float));
    assert(values != NULL);

    for (size_t i = 0; i < n; ++i) {
        const int rc = fscanf(file, "%f", &values[i]);
        assert(rc == 1);
    }

    fclose(file);
    *out_n = n;
    return values;
}

static inline char** point1_load_strings(const char* path, size_t* out_n) {
    FILE* file = fopen(path, "r");
    assert(file != NULL);

    const size_t n = point1_read_count(file);
    char** values = (char**)calloc(n, sizeof(char*));
    assert(values != NULL);

    char buffer[64];
    for (size_t i = 0; i < n; ++i) {
        const int rc = fscanf(file, "%63s", buffer);
        assert(rc == 1);
        const size_t len = strlen(buffer);
        values[i] = (char*)malloc(len + 1);
        assert(values[i] != NULL);
        memcpy(values[i], buffer, len + 1);
    }

    fclose(file);
    *out_n = n;
    return values;
}

static inline void point1_free_strings(char** values, size_t n) {
    if (values == NULL) {
        return;
    }

    for (size_t i = 0; i < n; ++i) {
        free(values[i]);
    }
    free(values);
}

static inline double point1_variance_u32(const uint32_t* values, size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sum += (double)values[i];
    }

    const double mean = sum / (double)n;
    double sq_sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double diff = (double)values[i] - mean;
        sq_sum += diff * diff;
    }

    return sq_sum / (double)n;
}

static inline uint32_t point1_min_u32(const uint32_t* values, size_t n) {
    assert(n > 0);
    uint32_t result = values[0];
    for (size_t i = 1; i < n; ++i) {
        if (values[i] < result) {
            result = values[i];
        }
    }
    return result;
}

static inline uint32_t point1_max_u32(const uint32_t* values, size_t n) {
    assert(n > 0);
    uint32_t result = values[0];
    for (size_t i = 1; i < n; ++i) {
        if (values[i] > result) {
            result = values[i];
        }
    }
    return result;
}

#endif
