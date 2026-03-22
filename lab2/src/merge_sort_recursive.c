#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "../include/point3_sorts.h"

static int* merge_ints(int* arr1, int* arr2, size_t size1, size_t size2, int* arr0) {
    assert(arr1 != NULL);
    assert(arr2 != NULL);
    assert(arr0 != NULL);

    int* res = (int*)calloc(size1 + size2, sizeof(int));
    if (res == NULL) {
        abort();
    }

    size_t added_elements1 = 0;
    size_t added_elements2 = 0;
    size_t index = 0;
    size_t all_size = size1 + size2;

    while (index < all_size) {
        if (added_elements1 == size1 || added_elements2 == size2) {
            break;
        }

        if (*arr1 < *arr2) {
            res[index] = *arr1;
            ++arr1;
            ++added_elements1;
        } else {
            res[index] = *arr2;
            ++arr2;
            ++added_elements2;
        }

        ++index;
    }

    if (added_elements1 == size1) {
        while (added_elements2 < size2) {
            res[index] = *arr2;
            ++arr2;
            ++index;
            ++added_elements2;
        }
    } else if (added_elements2 == size2) {
        while (added_elements1 < size1) {
            res[index] = *arr1;
            ++arr1;
            ++index;
            ++added_elements1;
        }
    }

    index = 0;
    while (index < all_size) {
        arr0[index] = res[index];
        ++index;
    }

    free(res);
    return arr0;
}

static int* merge_sort_recursive_impl(int* arr, size_t size) {
    assert(arr != NULL);

    if (size <= 1) {
        return arr;
    } else {
        int* arr1 = arr;
        int* arr2 = arr + size / 2;

        size_t size1 = size / 2;
        size_t size2 = size - size / 2;

        return merge_ints(
            merge_sort_recursive_impl(arr1, size1),
            merge_sort_recursive_impl(arr2, size2),
            size1,
            size2,
            arr
        );
    }
}

void merge_sort_recursive(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    merge_sort_recursive_impl(arr, n);
}