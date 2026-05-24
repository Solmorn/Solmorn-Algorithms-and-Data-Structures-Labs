#include "../include/point1_sorts.h"

static const size_t MIN_SORT_SIZE = 2u;

void lab2_swap_int(int* a, int* b);

void bubble_sort(int* arr, size_t n) {

    if (n < MIN_SORT_SIZE) {
        return;
    }

    for (size_t pass = 0; pass + 1u < n; pass++) {

        int swapped = 0;

        for (size_t i = 0; i + 1u < n - pass; i++) {

            if (arr[i] > arr[i + 1u]) {

                lab2_swap_int(&arr[i], &arr[i + 1u]);
                swapped = 1;
            }
        }

        if (!swapped) {
            break;
        }
    }
}
