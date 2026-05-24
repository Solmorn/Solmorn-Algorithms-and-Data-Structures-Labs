#ifndef POINT1_HASHES_H
#define POINT1_HASHES_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define POINT1_BUCKET_COUNT 1000u
#define POINT1_UINT_HASH_COUNT 3u
#define POINT1_FLOAT_HASH_COUNT 5u
#define POINT1_STRING_HASH_COUNT 4u

static inline uint32_t point1_u32_fold_bits(uint32_t value) {
    return value ^ (value >> 8) ^ (value >> 16) ^ (value >> 24);
}

static inline uint64_t point1_hash_uint_remainder(unsigned value) {
    return value;
}

static inline uint64_t point1_hash_uint_bit_repr(unsigned value) {
    return point1_u32_fold_bits(value);
}

static inline uint64_t point1_hash_uint_knuth(unsigned value) {
    const uint64_t multiplier = 2654435761u;
    return value * multiplier;
}

static inline uint64_t point1_float_bits(float value) {
    uint32_t bits = 0u;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static inline uint64_t point1_hash_float_cast_int_bits(float value) {
    int32_t as_int = (int32_t)value;
    uint32_t bits = 0u;
    memcpy(&bits, &as_int, sizeof(bits));
    return point1_u32_fold_bits(bits);
}

static inline uint64_t point1_hash_float_bits(float value) {
    return point1_float_bits(value);
}

static inline uint64_t point1_hash_float_mantissa(float value) {
    return point1_float_bits(value) & 0x7fffffu;
}

static inline uint64_t point1_hash_float_exponent(float value) {
    return (point1_float_bits(value) >> 23) & 0xffu;
}

static inline uint64_t point1_hash_float_mantissa_times_exponent(float value) {
    const uint64_t bits = point1_float_bits(value);
    const uint64_t mantissa = bits & 0x7fffffu;
    const uint64_t exponent = (bits >> 23) & 0xffu;
    return mantissa * (exponent + 1u);
}

static inline uint64_t point1_hash_string_length(const char* s) {
    return strlen(s);
}

static inline uint64_t point1_hash_string_sum(const char* s) {
    uint64_t sum = 0u;
    for (const unsigned char* p = (const unsigned char*)s; *p != '\0'; p++) {
        sum += *p;
    }
    return sum;
}

static inline uint64_t point1_hash_string_polynomial(const char* s) {
    const uint64_t base = 911382323ull;
    uint64_t hash = 0u;
    for (const unsigned char* p = (const unsigned char*)s; *p != '\0'; p++) {
        hash = hash * base + *p + 1ull;
    }
    return hash;
}

static inline void point1_crc32_init(uint32_t table[256]) {
    for (uint32_t i = 0u; i < 256u; i++) {
        uint32_t c = i;
        for (int bit = 0; bit < 8; bit++) {
            if ((c & 1u) != 0u) {
                c = 0xedb88320u ^ (c >> 1);
            } else {
                c >>= 1;
            }
        }
        table[i] = c;
    }
}

static inline uint32_t point1_crc32_hash(const char* s, const uint32_t table[256]) {
    uint32_t crc = 0xffffffffu;
    for (const unsigned char* p = (const unsigned char*)s; *p != '\0'; p++) {
        crc = table[(crc ^ *p) & 0xffu] ^ (crc >> 8);
    }
    return crc ^ 0xffffffffu;
}

#endif
