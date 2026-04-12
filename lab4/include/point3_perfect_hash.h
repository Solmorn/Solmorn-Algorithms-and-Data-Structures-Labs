#ifndef POINT3_PERFECT_HASH_H
#define POINT3_PERFECT_HASH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define POINT3_FKS_PRIME 4294967291ULL

typedef struct Point3SecondLevel {
    uint64_t a;
    uint64_t b;
    size_t key_count;
    size_t table_size;
    int* keys;
    unsigned char* used;
} Point3SecondLevel;

typedef struct Point3PerfectHash {
    uint64_t a;
    uint64_t b;
    size_t bucket_count;
    size_t key_count;
    Point3SecondLevel* buckets;
} Point3PerfectHash;

static inline uint32_t point3_rand_u32_local(void) {
    uint32_t x = (uint32_t)(rand() & 0x7fff);
    x = (x << 15) ^ (uint32_t)(rand() & 0x7fff);
    x = (x << 2) ^ (uint32_t)((unsigned)rand() & 0x3u);
    return x;
}

static inline size_t point3_universal_hash(int key, uint64_t a, uint64_t b, size_t m) {
    if (m == 0u) {
        return 0u;
    }
    const uint64_t x = (uint32_t)key;
    const uint64_t value = (a * x + b) % POINT3_FKS_PRIME;
    return (size_t)(value % (uint64_t)m);
}

static inline void point3_perfect_hash_init_empty(Point3PerfectHash* hash) {
    hash->a = 1u;
    hash->b = 0u;
    hash->bucket_count = 0u;
    hash->key_count = 0u;
    hash->buckets = NULL;
}

static inline void point3_second_level_destroy(Point3SecondLevel* bucket) {
    free(bucket->keys);
    free(bucket->used);
    bucket->keys = NULL;
    bucket->used = NULL;
    bucket->table_size = 0u;
    bucket->key_count = 0u;
    bucket->a = 1u;
    bucket->b = 0u;
}

static inline void point3_perfect_hash_destroy(Point3PerfectHash* hash) {
    if (hash->buckets != NULL) {
        for (size_t i = 0; i < hash->bucket_count; ++i) {
            point3_second_level_destroy(&hash->buckets[i]);
        }
    }
    free(hash->buckets);
    point3_perfect_hash_init_empty(hash);
}

static inline bool point3_build_second_level(Point3SecondLevel* bucket, const int* keys, size_t key_count) {
    bucket->key_count = key_count;

    if (key_count == 0u) {
        bucket->table_size = 0u;
        bucket->keys = NULL;
        bucket->used = NULL;
        bucket->a = 1u;
        bucket->b = 0u;
        return true;
    }

    if (key_count == 1u) {
        bucket->table_size = 1u;
        bucket->keys = (int*)malloc(sizeof(bucket->keys[0]));
        bucket->used = (unsigned char*)malloc(sizeof(bucket->used[0]));
        if (bucket->keys == NULL || bucket->used == NULL) {
            free(bucket->keys);
            free(bucket->used);
            bucket->keys = NULL;
            bucket->used = NULL;
            return false;
        }
        bucket->keys[0] = keys[0];
        bucket->used[0] = 1u;
        bucket->a = 1u;
        bucket->b = 0u;
        return true;
    }

    bucket->table_size = key_count * key_count;
    bucket->keys = (int*)malloc(bucket->table_size * sizeof(bucket->keys[0]));
    bucket->used = (unsigned char*)malloc(bucket->table_size * sizeof(bucket->used[0]));
    if (bucket->keys == NULL || bucket->used == NULL) {
        free(bucket->keys);
        free(bucket->used);
        bucket->keys = NULL;
        bucket->used = NULL;
        return false;
    }

    for (size_t attempt = 0; attempt < 10000u; ++attempt) {
        bucket->a = 1u + ((uint64_t)point3_rand_u32_local() % (POINT3_FKS_PRIME - 1u));
        bucket->b = (uint64_t)point3_rand_u32_local() % POINT3_FKS_PRIME;

        memset(bucket->used, 0, bucket->table_size * sizeof(bucket->used[0]));

        bool ok = true;
        for (size_t i = 0; i < key_count; ++i) {
            const size_t idx = point3_universal_hash(keys[i], bucket->a, bucket->b, bucket->table_size);
            if (bucket->used[idx]) {
                ok = false;
                break;
            }
            bucket->used[idx] = 1u;
            bucket->keys[idx] = keys[i];
        }

        if (ok) {
            return true;
        }
    }

    free(bucket->keys);
    free(bucket->used);
    bucket->keys = NULL;
    bucket->used = NULL;
    bucket->table_size = 0u;
    return false;
}

static inline bool point3_perfect_hash_build(Point3PerfectHash* hash, const int* keys, size_t key_count) {
    point3_perfect_hash_destroy(hash);

    hash->key_count = key_count;
    hash->bucket_count = (key_count == 0u) ? 1u : key_count;

    size_t* counts = (size_t*)calloc(hash->bucket_count, sizeof(counts[0]));
    size_t* offsets = (size_t*)malloc(hash->bucket_count * sizeof(offsets[0]));
    size_t* cursor = (size_t*)malloc(hash->bucket_count * sizeof(cursor[0]));
    int* grouped_keys = (int*)malloc(key_count * sizeof(grouped_keys[0]));

    if (counts == NULL || offsets == NULL || cursor == NULL || grouped_keys == NULL) {
        free(counts);
        free(offsets);
        free(cursor);
        free(grouped_keys);
        return false;
    }

    for (size_t attempt = 0; attempt < 10000u; ++attempt) {
        memset(counts, 0, hash->bucket_count * sizeof(counts[0]));

        hash->a = 1u + ((uint64_t)point3_rand_u32_local() % (POINT3_FKS_PRIME - 1u));
        hash->b = (uint64_t)point3_rand_u32_local() % POINT3_FKS_PRIME;

        for (size_t i = 0; i < key_count; ++i) {
            const size_t bucket = point3_universal_hash(keys[i], hash->a, hash->b, hash->bucket_count);
            ++counts[bucket];
        }

        size_t sum_sq = 0u;
        for (size_t i = 0; i < hash->bucket_count; ++i) {
            sum_sq += counts[i] * counts[i];
        }

        if (sum_sq <= 4u * (key_count == 0u ? 1u : key_count)) {
            break;
        }

        if (attempt + 1u == 10000u) {
            free(counts);
            free(offsets);
            free(cursor);
            free(grouped_keys);
            return false;
        }
    }

    offsets[0] = 0u;
    for (size_t i = 1; i < hash->bucket_count; ++i) {
        offsets[i] = offsets[i - 1u] + counts[i - 1u];
    }
    memcpy(cursor, offsets, hash->bucket_count * sizeof(cursor[0]));

    for (size_t i = 0; i < key_count; ++i) {
        const size_t bucket = point3_universal_hash(keys[i], hash->a, hash->b, hash->bucket_count);
        grouped_keys[cursor[bucket]++] = keys[i];
    }

    hash->buckets = (Point3SecondLevel*)calloc(hash->bucket_count, sizeof(hash->buckets[0]));
    if (hash->buckets == NULL) {
        free(counts);
        free(offsets);
        free(cursor);
        free(grouped_keys);
        return false;
    }

    for (size_t i = 0; i < hash->bucket_count; ++i) {
        const int* bucket_keys = grouped_keys + offsets[i];
        if (!point3_build_second_level(&hash->buckets[i], bucket_keys, counts[i])) {
            free(counts);
            free(offsets);
            free(cursor);
            free(grouped_keys);
            point3_perfect_hash_destroy(hash);
            return false;
        }
    }

    free(counts);
    free(offsets);
    free(cursor);
    free(grouped_keys);
    return true;
}

static inline bool point3_perfect_hash_contains(const Point3PerfectHash* hash, int key) {
    if (hash->bucket_count == 0u || hash->buckets == NULL) {
        return false;
    }

    const size_t first = point3_universal_hash(key, hash->a, hash->b, hash->bucket_count);
    const Point3SecondLevel* bucket = &hash->buckets[first];

    if (bucket->table_size == 0u || bucket->used == NULL || bucket->keys == NULL) {
        return false;
    }

    const size_t second = point3_universal_hash(key, bucket->a, bucket->b, bucket->table_size);
    return bucket->used[second] && bucket->keys[second] == key;
}

#endif