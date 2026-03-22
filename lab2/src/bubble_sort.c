#include "../include/point1_sorts.h"

void bubble_sort(int* arr, size_t n) {

    if (n < 2) {
        return;
    }

    for (size_t pass = 0; pass + 1 < n; ++pass) {

        int swapped = 0;

        for (size_t i = 0; i + 1 < n - pass; ++i) {

            if (arr[i] > arr[i + 1]) {

                int temp = arr[i];

                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                
                swapped = 1;
            }
        }

        if (!swapped) {
            break;
        }
    }
}