#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../include/point9_sorts.h"

#define RADIX_BASE 256
#define BYTE_COUNT 4

static unsigned int get_byte(uint32_t value, unsigned int shift) {
    return (value >> shift) & 0xffu;
}


void lsd_radix_sort(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    int* buffer = (int*)calloc(n, sizeof(int));
    if (buffer == NULL) {
        abort();
    }

    for (unsigned int byte_index = 0; byte_index < BYTE_COUNT; ++byte_index) {
        unsigned int shift = byte_index * 8;
        size_t pref_cnt[RADIX_BASE];
        memset(pref_cnt, 0, sizeof(pref_cnt));

        for (size_t i = 0; i < n; ++i) {
            uint32_t value = (uint32_t)arr[i];
            ++pref_cnt[get_byte(value, shift)];
        }

        for (size_t i = 1; i < RADIX_BASE; ++i) {
            pref_cnt[i] += pref_cnt[i - 1];
        }

        for (size_t i = n; i > 0; --i) {
            uint32_t value = (uint32_t)arr[i - 1];
            unsigned int digit = get_byte(value, shift);
            buffer[--pref_cnt[digit]] = arr[i - 1];
        }

        memcpy(arr, buffer, n * sizeof(int));
    }

    free(buffer);
}

static void msd_radix_sort_impl(int* arr, int* buffer, size_t left, size_t right, unsigned int shift) {
    if (right - left < 2) {
        return;
    }

    size_t count[RADIX_BASE];
    size_t start[RADIX_BASE];
    size_t next_pos[RADIX_BASE];

    memset(count, 0, sizeof(count));

    for (size_t i = left; i < right; ++i) {
        uint32_t value = (uint32_t)arr[i];
        ++count[get_byte(value, shift)];
    }

    start[0] = left;
    for (size_t i = 1; i < RADIX_BASE; ++i) {
        start[i] = start[i - 1] + count[i - 1];
    }

    for (size_t i = 0; i < RADIX_BASE; ++i) {
        next_pos[i] = start[i];
    }

    for (size_t i = left; i < right; ++i) {
        uint32_t value = (uint32_t)arr[i];
        unsigned int digit = get_byte(value, shift);
        buffer[next_pos[digit]++] = arr[i];
    }

    memcpy(arr + left, buffer + left, (right - left) * sizeof(int));

    if (shift == 0) {
        return;
    }

    unsigned int next_shift = shift - 8;

    for (size_t digit = 0; digit < RADIX_BASE; ++digit) {
        size_t bucket_left = start[digit];
        size_t bucket_right = bucket_left + count[digit];

        if (bucket_right - bucket_left > 1) {
            msd_radix_sort_impl(arr, buffer, bucket_left, bucket_right, next_shift);
        }
    }
}

void msd_radix_sort(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    int* buffer = (int*)calloc(n, sizeof(int));
    if (buffer == NULL) {
        abort();
    }

    msd_radix_sort_impl(arr, buffer, 0, n, 24);

    free(buffer);
}