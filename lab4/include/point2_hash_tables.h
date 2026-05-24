#ifndef POINT2_HASH_TABLES_H
#define POINT2_HASH_TABLES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define POINT2_EMPTY   0u
#define POINT2_FILLED  1u
#define POINT2_DELETED 2u

#define POINT2_LINEAR_MODE     1
#define POINT2_QUADRATIC_MODE  2
#define POINT2_DOUBLE_MODE     3

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

typedef struct Point2ProbeTable {
    int* keys;
    unsigned char* states;
    size_t capacity;
    size_t size;
    double max_load_factor;
    int mode;
} Point2ProbeTable;

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

uint32_t point2_mix_u32(uint32_t x);
uint32_t point2_hash_int_primary(int key);
uint32_t point2_hash_int_secondary(int key);
size_t point2_probe_index(uint32_t h1, uint32_t h2, size_t cap, size_t step_i, int mode);
bool point2_is_prime(size_t n);
size_t point2_next_prime(size_t n);

bool point2_chain_init(Point2ChainTable* table, double max_load_factor);
void point2_chain_destroy(Point2ChainTable* table);
bool point2_chain_find(const Point2ChainTable* table, int key);
bool point2_chain_rehash(Point2ChainTable* table, size_t new_capacity);
bool point2_chain_insert(Point2ChainTable* table, int key);
bool point2_chain_erase(Point2ChainTable* table, int key);

bool point2_probe_init(Point2ProbeTable* table, int mode, double max_load_factor);
void point2_probe_destroy(Point2ProbeTable* table);
bool point2_probe_contains(const Point2ProbeTable* table, int key);
bool point2_probe_rehash(Point2ProbeTable* table, size_t new_capacity);
bool point2_probe_insert(Point2ProbeTable* table, int key);
bool point2_probe_erase(Point2ProbeTable* table, int key);

bool point2_cuckoo_init(Point2CuckooTable* table, double max_load_factor);
void point2_cuckoo_destroy(Point2CuckooTable* table);
bool point2_cuckoo_contains(const Point2CuckooTable* table, int key);
bool point2_cuckoo_insert(Point2CuckooTable* table, int key);
bool point2_cuckoo_erase(Point2CuckooTable* table, int key);
bool point2_cuckoo_rebuild(Point2CuckooTable* table, size_t min_capacity, const int* extra_key, bool has_extra);

#endif
