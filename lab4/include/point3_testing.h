#ifndef POINT3_TESTING_H
#define POINT3_TESTING_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "point2_hash_tables.h"

#define POINT3_KEY_COUNT 100000u
#define POINT3_QUERY_COUNT 10000000u

static inline double point3_now_seconds(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static inline uint32_t point3_rand_u32(void) {
    uint32_t a = (uint32_t)(rand() & 0x7fff);
    uint32_t b = (uint32_t)(rand() & 0x7fff);
    uint32_t c = (uint32_t)(rand() & 0x7fff);
    return (a << 17) ^ (b << 2) ^ c;
}

static inline int point3_rand_int(void) {
    return (int)point3_rand_u32();
}

static inline bool point3_generate_distinct_keys(int* keys, size_t key_count) {
    Point2ProbeTable used;
    point2_probe_init(&used, POINT2_DOUBLE_MODE, 0.70);

    size_t written = 0u;
    while (written < key_count) {
        const int candidate = point3_rand_int();
        if (!point2_probe_insert(&used, candidate)) {
            continue;
        }
        keys[written++] = candidate;
    }

    point2_probe_destroy(&used);
    return true;
}

static inline bool point3_generate_queries(const int* keys, size_t key_count, int* queries, size_t query_count) {
    Point2ProbeTable present;
    point2_probe_init(&present, POINT2_DOUBLE_MODE, 0.70);

    for (size_t i = 0; i < key_count; ++i) {
        (void)point2_probe_insert(&present, keys[i]);
    }

    for (size_t i = 0; i < query_count; ++i) {
        if ((i & 1u) == 0u) {
            queries[i] = keys[(size_t)(point3_rand_u32() % (uint32_t)key_count)];
        } else {
            int candidate = point3_rand_int();
            while (point2_probe_contains(&present, candidate)) {
                candidate = point3_rand_int();
            }
            queries[i] = candidate;
        }
    }

    point2_probe_destroy(&present);
    return true;
}

#endif