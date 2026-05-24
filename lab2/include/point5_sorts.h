#ifndef POINT5_SORTS_H
#define POINT5_SORTS_H

#include <stddef.h>

void quick_sort_pivot_middle(int* arr, size_t n);
void quick_sort_pivot_median3(int* arr, size_t n);
void quick_sort_pivot_random(int* arr, size_t n);
void quick_sort_pivot_median3_random(int* arr, size_t n);
void quick_sort_pivot_median_of_medians(int* arr, size_t n);

#endif