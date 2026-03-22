#ifndef POINT7_SORTS_H
#define POINT7_SORTS_H

#include <stddef.h>

void quick_sort_best_cutoff32(int* arr, size_t n);

void introsort_c1(int* arr, size_t n);
void introsort_c2(int* arr, size_t n);
void introsort_c3(int* arr, size_t n);
void introsort_c4(int* arr, size_t n);

void introsort_best(int* arr, size_t n);

#endif