#include "../include/point1_sorts.h"

void shell_sort(int* arr, size_t n) {
    
    size_t step = 1;

    while (step <= n / 9) {
        step = step * 3 + 1;
    }

    for (; step > 0; step /= 3) {

        for (size_t i = step; i < n; ++i) {
            size_t j = i;
            int tmp = arr[i];

            while (j >= step && arr[j - step] > tmp) {
                arr[j] = arr[j - step];
                j -= step;
            }

            arr[j] = tmp;
        }

        if (step == 1) {
            break;
        }
    }
}