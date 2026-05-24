#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../include/point3_sorts.h"

static const size_t MIN_SORT_SIZE = 2u;
static const size_t MIDDLE_DIVISOR = 2u;

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

static void merge_sort_recursive_impl(int* arr, size_t left, size_t right, int* res) {
    assert(arr != NULL);
    assert(res != NULL);

    if (right - left <= 1u) {
        return;
    }

    size_t middle = left + (right - left) / MIDDLE_DIVISOR;

    merge_sort_recursive_impl(arr, left, middle, res);
    merge_sort_recursive_impl(arr, middle, right, res);
    merge_range(arr, left, middle, right, res);
}

void merge_sort_recursive(int* arr, size_t n) {
    if (n < MIN_SORT_SIZE) {
        return;
    }

    int* res = (int*)malloc(n * sizeof(res[0]));
    if (res == NULL) {
        return;
    }

    merge_sort_recursive_impl(arr, 0u, n, res);
    free(res);
}
