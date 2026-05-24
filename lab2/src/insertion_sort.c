#include "../include/point1_sorts.h"

void insertion_sort(int* arr, size_t n) {

    for (size_t i = 1; i < n; ++i) {

        int value = arr[i];
        size_t j = i;

        while (j > 0 && arr[j - 1] > value) {
            
            arr[j] = arr[j - 1];
            --j;
        }

        arr[j] = value;
    }
}