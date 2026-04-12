#ifndef POINT2_HASH_TABLES_H
#define POINT2_HASH_TABLES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define POINT2_EMPTY   0u
#define POINT2_FILLED  1u
#define POINT2_DELETED 2u

#define POINT2_LINEAR_MODE     1
#define POINT2_QUADRATIC_MODE  2
#define POINT2_DOUBLE_MODE     3

static inline uint32_t point2_mix_u32(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352du;
    x ^= x >> 15;
    x *= 0x846ca68bu;
    x ^= x >> 16;
    return x;
}

static inline uint32_t point2_hash_int_primary(int key) {
    return point2_mix_u32((uint32_t)key);
}

static inline uint32_t point2_hash_int_secondary(int key) {
    return point2_mix_u32((uint32_t)key ^ 0x9e3779b9u);
}

static inline size_t point2_probe_index(uint32_t h1, uint32_t h2, size_t cap, size_t step_i, int mode) {
    switch (mode) {
        case POINT2_LINEAR_MODE:
            return (size_t)((h1 + (uint32_t)step_i) % (uint32_t)cap);
        case POINT2_QUADRATIC_MODE: {
            const uint64_t add = (uint64_t)step_i + (uint64_t)step_i * (uint64_t)step_i;
            return (size_t)((h1 + add) % (uint64_t)cap);
        }
        case POINT2_DOUBLE_MODE:
            return (size_t)((h1 + (uint64_t)step_i * (uint64_t)h2) % (uint64_t)cap);
        default:
            return 0;
    }
}

static inline bool point2_is_prime(size_t n) {
    if (n < 2u) {
        return false;
    }
    if ((n % 2u) == 0u) {
        return n == 2u;
    }
    for (size_t d = 3u; d * d <= n; d += 2u) {
        if ((n % d) == 0u) {
            return false;
        }
    }
    return true;
}

static inline size_t point2_next_prime(size_t n) {
    if (n <= 2u) {
        return 2u;
    }
    if ((n % 2u) == 0u) {
        ++n;
    }
    while (!point2_is_prime(n)) {
        n += 2u;
    }
    return n;
}

typedef struct Point2ChainNode {
    int key;
    struct Point2ChainNode* next;
} Point2ChainNode;

typedef struct Point2ChainTable {
    Point2ChainNode** buckets;
    size_t capacity;
    size_t size;
    double max_load_factor;
} Point2ChainTable;

static inline void point2_chain_init(Point2ChainTable* table, double max_load_factor) {
    table->capacity = 1021u;
    table->size = 0u;
    table->max_load_factor = max_load_factor;
    table->buckets = (Point2ChainNode**)calloc(table->capacity, sizeof(table->buckets[0]));
}

static inline void point2_chain_destroy(Point2ChainTable* table) {
    if (table->buckets == NULL) {
        return;
    }
    for (size_t i = 0; i < table->capacity; ++i) {
        Point2ChainNode* cur = table->buckets[i];
        while (cur != NULL) {
            Point2ChainNode* next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(table->buckets);
    table->buckets = NULL;
    table->capacity = 0u;
    table->size = 0u;
}

static inline bool point2_chain_find(const Point2ChainTable* table, int key) {
    const size_t idx = (size_t)(point2_hash_int_primary(key) % (uint32_t)table->capacity);
    Point2ChainNode* cur = table->buckets[idx];
    while (cur != NULL) {
        if (cur->key == key) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

static inline bool point2_chain_rehash(Point2ChainTable* table, size_t new_capacity) {
    Point2ChainNode** new_buckets = (Point2ChainNode**)calloc(new_capacity, sizeof(new_buckets[0]));
    if (new_buckets == NULL) {
        return false;
    }

    for (size_t i = 0; i < table->capacity; ++i) {
        Point2ChainNode* cur = table->buckets[i];
        while (cur != NULL) {
            Point2ChainNode* next = cur->next;
            const size_t idx = (size_t)(point2_hash_int_primary(cur->key) % (uint32_t)new_capacity);
            cur->next = new_buckets[idx];
            new_buckets[idx] = cur;
            cur = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->capacity = new_capacity;
    return true;
}

static inline bool point2_chain_insert(Point2ChainTable* table, int key) {
    if (((double)(table->size + 1u) / (double)table->capacity) > table->max_load_factor) {
        const size_t new_capacity = point2_next_prime(table->capacity * 2u + 1u);
        if (!point2_chain_rehash(table, new_capacity)) {
            return false;
        }
    }

    const size_t idx = (size_t)(point2_hash_int_primary(key) % (uint32_t)table->capacity);
    Point2ChainNode* cur = table->buckets[idx];
    while (cur != NULL) {
        if (cur->key == key) {
            return false;
        }
        cur = cur->next;
    }

    Point2ChainNode* node = (Point2ChainNode*)malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->key = key;
    node->next = table->buckets[idx];
    table->buckets[idx] = node;
    ++table->size;
    return true;
}

static inline bool point2_chain_erase(Point2ChainTable* table, int key) {
    const size_t idx = (size_t)(point2_hash_int_primary(key) % (uint32_t)table->capacity);
    Point2ChainNode* cur = table->buckets[idx];
    Point2ChainNode* prev = NULL;
    while (cur != NULL) {
        if (cur->key == key) {
            if (prev == NULL) {
                table->buckets[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            --table->size;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

typedef struct Point2ProbeTable {
    int* keys;
    unsigned char* states;
    size_t capacity;
    size_t size;
    double max_load_factor;
    int mode;
} Point2ProbeTable;

static inline void point2_probe_init(Point2ProbeTable* table, int mode, double max_load_factor) {
    table->capacity = 1031u;
    table->size = 0u;
    table->max_load_factor = max_load_factor;
    table->mode = mode;
    table->keys = (int*)malloc(table->capacity * sizeof(table->keys[0]));
    table->states = (unsigned char*)calloc(table->capacity, sizeof(table->states[0]));
}

static inline void point2_probe_destroy(Point2ProbeTable* table) {
    free(table->keys);
    free(table->states);
    table->keys = NULL;
    table->states = NULL;
    table->capacity = 0u;
    table->size = 0u;
}

static inline bool point2_probe_contains(const Point2ProbeTable* table, int key) {
    const uint32_t h1 = point2_hash_int_primary(key) % (uint32_t)table->capacity;
    uint32_t h2 = 1u;
    if (table->mode == POINT2_DOUBLE_MODE) {
        h2 = 1u + point2_hash_int_secondary(key) % (uint32_t)(table->capacity - 1u);
    }

    for (size_t i = 0; i < table->capacity; ++i) {
        const size_t idx = point2_probe_index(h1, h2, table->capacity, i, table->mode);
        if (table->states[idx] == POINT2_EMPTY) {
            return false;
        }
        if (table->states[idx] == POINT2_FILLED && table->keys[idx] == key) {
            return true;
        }
    }
    return false;
}

static inline bool point2_probe_rehash(Point2ProbeTable* table, size_t new_capacity) {
    Point2ProbeTable fresh;
    fresh.capacity = point2_next_prime(new_capacity);
    fresh.size = 0u;
    fresh.max_load_factor = table->max_load_factor;
    fresh.mode = table->mode;
    fresh.keys = (int*)malloc(fresh.capacity * sizeof(fresh.keys[0]));
    fresh.states = (unsigned char*)calloc(fresh.capacity, sizeof(fresh.states[0]));
    if (fresh.keys == NULL || fresh.states == NULL) {
        free(fresh.keys);
        free(fresh.states);
        return false;
    }

    for (size_t i = 0; i < table->capacity; ++i) {
        if (table->states[i] != POINT2_FILLED) {
            continue;
        }
        const int key = table->keys[i];
        const uint32_t h1 = point2_hash_int_primary(key) % (uint32_t)fresh.capacity;
        uint32_t h2 = 1u;
        if (fresh.mode == POINT2_DOUBLE_MODE) {
            h2 = 1u + point2_hash_int_secondary(key) % (uint32_t)(fresh.capacity - 1u);
        }
        for (size_t step = 0; step < fresh.capacity; ++step) {
            const size_t idx = point2_probe_index(h1, h2, fresh.capacity, step, fresh.mode);
            if (fresh.states[idx] != POINT2_FILLED) {
                fresh.keys[idx] = key;
                fresh.states[idx] = POINT2_FILLED;
                ++fresh.size;
                break;
            }
        }
    }

    free(table->keys);
    free(table->states);
    *table = fresh;
    return true;
}

static inline bool point2_probe_insert(Point2ProbeTable* table, int key) {
    if (((double)(table->size + 1u) / (double)table->capacity) > table->max_load_factor) {
        if (!point2_probe_rehash(table, table->capacity * 2u + 1u)) {
            return false;
        }
    }

    while (true) {
        const uint32_t h1 = point2_hash_int_primary(key) % (uint32_t)table->capacity;
        uint32_t h2 = 1u;
        if (table->mode == POINT2_DOUBLE_MODE) {
            h2 = 1u + point2_hash_int_secondary(key) % (uint32_t)(table->capacity - 1u);
        }

        size_t first_deleted = (size_t)(-1);
        for (size_t step = 0; step < table->capacity; ++step) {
            const size_t idx = point2_probe_index(h1, h2, table->capacity, step, table->mode);
            if (table->states[idx] == POINT2_FILLED) {
                if (table->keys[idx] == key) {
                    return false;
                }
                continue;
            }
            if (table->states[idx] == POINT2_DELETED) {
                if (first_deleted == (size_t)(-1)) {
                    first_deleted = idx;
                }
                continue;
            }
            if (table->states[idx] == POINT2_EMPTY) {
                const size_t dst = (first_deleted == (size_t)(-1)) ? idx : first_deleted;
                table->keys[dst] = key;
                table->states[dst] = POINT2_FILLED;
                ++table->size;
                return true;
            }
        }

        if (first_deleted != (size_t)(-1)) {
            table->keys[first_deleted] = key;
            table->states[first_deleted] = POINT2_FILLED;
            ++table->size;
            return true;
        }

        if (!point2_probe_rehash(table, table->capacity * 2u + 1u)) {
            return false;
        }
    }
}

static inline bool point2_probe_erase(Point2ProbeTable* table, int key) {
    const uint32_t h1 = point2_hash_int_primary(key) % (uint32_t)table->capacity;
    uint32_t h2 = 1u;
    if (table->mode == POINT2_DOUBLE_MODE) {
        h2 = 1u + point2_hash_int_secondary(key) % (uint32_t)(table->capacity - 1u);
    }

    for (size_t i = 0; i < table->capacity; ++i) {
        const size_t idx = point2_probe_index(h1, h2, table->capacity, i, table->mode);
        if (table->states[idx] == POINT2_EMPTY) {
            return false;
        }
        if (table->states[idx] == POINT2_FILLED && table->keys[idx] == key) {
            table->states[idx] = POINT2_DELETED;
            --table->size;
            return true;
        }
    }
    return false;
}

typedef struct Point2CuckooTable {
    int* keys1;
    int* keys2;
    unsigned char* used1;
    unsigned char* used2;
    size_t capacity;
    size_t size;
    double max_load_factor;
    uint32_t seed1;
    uint32_t seed2;
} Point2CuckooTable;

static inline size_t point2_cuckoo_idx1_with_seed(int key, size_t capacity, uint32_t seed) {
    return (size_t)(point2_mix_u32((uint32_t)key ^ seed) % (uint32_t)capacity);
}

static inline size_t point2_cuckoo_idx2_with_seed(int key, size_t capacity, uint32_t seed) {
    return (size_t)(point2_mix_u32(((uint32_t)key * 0x9e3779b9u) ^ seed) % (uint32_t)capacity);
}

static inline bool point2_cuckoo_try_build(
    const int* keys,
    size_t key_count,
    size_t capacity,
    uint32_t seed1,
    uint32_t seed2,
    int* keys1,
    int* keys2,
    unsigned char* used1,
    unsigned char* used2
) {
    memset(used1, 0, capacity * sizeof(used1[0]));
    memset(used2, 0, capacity * sizeof(used2[0]));

    const size_t max_kicks = 128u;
    for (size_t i = 0; i < key_count; ++i) {
        int cur = keys[i];
        int table_id = 1;
        for (size_t kick = 0; kick < max_kicks; ++kick) {
            if (table_id == 1) {
                const size_t idx = point2_cuckoo_idx1_with_seed(cur, capacity, seed1);
                if (!used1[idx]) {
                    keys1[idx] = cur;
                    used1[idx] = 1u;
                    break;
                }
                if (keys1[idx] == cur) {
                    break;
                }
                const int tmp = keys1[idx];
                keys1[idx] = cur;
                cur = tmp;
                table_id = 2;
            } else {
                const size_t idx = point2_cuckoo_idx2_with_seed(cur, capacity, seed2);
                if (!used2[idx]) {
                    keys2[idx] = cur;
                    used2[idx] = 1u;
                    break;
                }
                if (keys2[idx] == cur) {
                    break;
                }
                const int tmp = keys2[idx];
                keys2[idx] = cur;
                cur = tmp;
                table_id = 1;
            }
            if (kick + 1u == max_kicks) {
                return false;
            }
        }
    }
    return true;
}

static inline bool point2_cuckoo_rebuild(Point2CuckooTable* table, size_t min_capacity, const int* extra_key, bool has_extra) {
    const size_t total_keys = table->size + (has_extra ? 1u : 0u);
    int* all_keys = (int*)malloc(total_keys * sizeof(all_keys[0]));
    if (all_keys == NULL && total_keys > 0u) {
        return false;
    }

    size_t pos = 0u;
    for (size_t i = 0; i < table->capacity; ++i) {
        if (table->used1 != NULL && table->used1[i]) {
            all_keys[pos++] = table->keys1[i];
        }
        if (table->used2 != NULL && table->used2[i]) {
            all_keys[pos++] = table->keys2[i];
        }
    }
    if (has_extra) {
        all_keys[pos++] = *extra_key;
    }

    size_t capacity = point2_next_prime(min_capacity < 1031u ? 1031u : min_capacity);
    for (int attempt = 0; attempt < 64; ++attempt) {
        int* new_keys1 = (int*)malloc(capacity * sizeof(new_keys1[0]));
        int* new_keys2 = (int*)malloc(capacity * sizeof(new_keys2[0]));
        unsigned char* new_used1 = (unsigned char*)calloc(capacity, sizeof(new_used1[0]));
        unsigned char* new_used2 = (unsigned char*)calloc(capacity, sizeof(new_used2[0]));
        if (new_keys1 == NULL || new_keys2 == NULL || new_used1 == NULL || new_used2 == NULL) {
            free(new_keys1);
            free(new_keys2);
            free(new_used1);
            free(new_used2);
            free(all_keys);
            return false;
        }

        const uint32_t seed1 = 0x1234567u + (uint32_t)attempt * 0x9e3779b9u;
        const uint32_t seed2 = 0x89abcdefu ^ ((uint32_t)attempt * 0x85ebca6bu);
        if (point2_cuckoo_try_build(all_keys, pos, capacity, seed1, seed2, new_keys1, new_keys2, new_used1, new_used2)) {
            free(table->keys1);
            free(table->keys2);
            free(table->used1);
            free(table->used2);

            table->keys1 = new_keys1;
            table->keys2 = new_keys2;
            table->used1 = new_used1;
            table->used2 = new_used2;
            table->capacity = capacity;
            table->size = pos;
            table->seed1 = seed1;
            table->seed2 = seed2;
            free(all_keys);
            return true;
        }

        free(new_keys1);
        free(new_keys2);
        free(new_used1);
        free(new_used2);

        if ((attempt % 4) == 3) {
            capacity = point2_next_prime(capacity * 2u + 1u);
        }
    }

    free(all_keys);
    return false;
}

static inline void point2_cuckoo_init(Point2CuckooTable* table, double max_load_factor) {
    table->keys1 = NULL;
    table->keys2 = NULL;
    table->used1 = NULL;
    table->used2 = NULL;
    table->capacity = 0u;
    table->size = 0u;
    table->max_load_factor = max_load_factor;
    table->seed1 = 0x1234567u;
    table->seed2 = 0x89abcdefu;
    (void)point2_cuckoo_rebuild(table, 1031u, NULL, false);
}

static inline void point2_cuckoo_destroy(Point2CuckooTable* table) {
    free(table->keys1);
    free(table->keys2);
    free(table->used1);
    free(table->used2);
    table->keys1 = NULL;
    table->keys2 = NULL;
    table->used1 = NULL;
    table->used2 = NULL;
    table->capacity = 0u;
    table->size = 0u;
}

static inline bool point2_cuckoo_contains(const Point2CuckooTable* table, int key) {
    if (table->capacity == 0u) {
        return false;
    }
    const size_t i1 = point2_cuckoo_idx1_with_seed(key, table->capacity, table->seed1);
    if (table->used1[i1] && table->keys1[i1] == key) {
        return true;
    }
    const size_t i2 = point2_cuckoo_idx2_with_seed(key, table->capacity, table->seed2);
    return table->used2[i2] && table->keys2[i2] == key;
}

static inline bool point2_cuckoo_insert(Point2CuckooTable* table, int key) {
    if (point2_cuckoo_contains(table, key)) {
        return false;
    }

    const double projected_load = (double)(table->size + 1u) / (double)(table->capacity * 2u);
    if (projected_load > table->max_load_factor) {
        if (!point2_cuckoo_rebuild(table, table->capacity * 2u + 1u, &key, true)) {
            return false;
        }
        return true;
    }

    int cur = key;
    int table_id = 1;
    const size_t max_kicks = 128u;
    for (size_t kick = 0; kick < max_kicks; ++kick) {
        if (table_id == 1) {
            const size_t idx = point2_cuckoo_idx1_with_seed(cur, table->capacity, table->seed1);
            if (!table->used1[idx]) {
                table->keys1[idx] = cur;
                table->used1[idx] = 1u;
                ++table->size;
                return true;
            }
            const int tmp = table->keys1[idx];
            table->keys1[idx] = cur;
            cur = tmp;
            table_id = 2;
        } else {
            const size_t idx = point2_cuckoo_idx2_with_seed(cur, table->capacity, table->seed2);
            if (!table->used2[idx]) {
                table->keys2[idx] = cur;
                table->used2[idx] = 1u;
                ++table->size;
                return true;
            }
            const int tmp = table->keys2[idx];
            table->keys2[idx] = cur;
            cur = tmp;
            table_id = 1;
        }
    }

    return point2_cuckoo_rebuild(table, table->capacity * 2u + 1u, &cur, true);
}

static inline bool point2_cuckoo_erase(Point2CuckooTable* table, int key) {
    if (table->capacity == 0u) {
        return false;
    }
    const size_t i1 = point2_cuckoo_idx1_with_seed(key, table->capacity, table->seed1);
    if (table->used1[i1] && table->keys1[i1] == key) {
        table->used1[i1] = 0u;
        --table->size;
        return true;
    }
    const size_t i2 = point2_cuckoo_idx2_with_seed(key, table->capacity, table->seed2);
    if (table->used2[i2] && table->keys2[i2] == key) {
        table->used2[i2] = 0u;
        --table->size;
        return true;
    }
    return false;
}

#endif

