#include "../include/point1_sorts.h"

void lab2_swap_int(int* a, int* b);

void selection_sort(int* arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        size_t min_index = i;

        for (size_t j = i + 1u; j < n; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }

        if (min_index != i) {
            lab2_swap_int(&arr[i], &arr[min_index]);
        }
    }
}
