#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../include/point3_sorts.h"

static const size_t MIN_SORT_SIZE = 2u;
static const size_t MERGE_BLOCK_MULTIPLIER = 2u;

static size_t min_size(size_t a, size_t b) {
    return (a < b) ? a : b;
}

static void merge_range(int* arr, size_t left, size_t middle, size_t right, int* res) {
    assert(arr != NULL);
    assert(res != NULL);
    assert(left <= middle);
    assert(middle <= right);

    size_t left_pos = left;
    size_t right_pos = middle;
    size_t res_pos = left;

    while (left_pos < middle && right_pos < right) {
        if (arr[left_pos] < arr[right_pos]) {
            res[res_pos] = arr[left_pos];
            left_pos++;
        } else {
            res[res_pos] = arr[right_pos];
            right_pos++;
        }

        res_pos++;
    }

    while (left_pos < middle) {
        res[res_pos] = arr[left_pos];
        left_pos++;
        res_pos++;
    }

    while (right_pos < right) {
        res[res_pos] = arr[right_pos];
        right_pos++;
        res_pos++;
    }

    for (size_t i = left; i < right; i++) {
        arr[i] = res[i];
    }
}

void merge_sort_iterative(int* arr, size_t n) {
    if (n < MIN_SORT_SIZE) {
        return;
    }

    int* res = (int*)malloc(n * sizeof(res[0]));
    if (res == NULL) {
        return;
    }

    for (size_t width = 1u; width < n; width *= MERGE_BLOCK_MULTIPLIER) {
        for (size_t left = 0u; left < n; left += MERGE_BLOCK_MULTIPLIER * width) {
            size_t middle = min_size(left + width, n);
            size_t right = min_size(left + MERGE_BLOCK_MULTIPLIER * width, n);

            if (middle == left || middle == right) {
                continue;
            }

            merge_range(arr, left, middle, right, res);
        }
    }

    free(res);
}
