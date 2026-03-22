#include "../include/point8_sorts.h"

#include "../third_party/pdqsort/pdqsort.h"
#include "../third_party/cpp-TimSort/include/gfx/timsort.hpp"

void timsort_github(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    gfx::timsort(arr, arr + n);
}

void pdqsort_github(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    pdqsort(arr, arr + n);
}