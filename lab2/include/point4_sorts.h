#ifndef POINT4_SORTS_H
#define POINT4_SORTS_H

#include <stddef.h>

void quick_sort_lomuto(int* arr, size_t n);
void quick_sort_hoare(int* arr, size_t n);
void quick_sort_fat(int* arr, size_t n);

void quick_sort_fat_one_branch(int* arr, size_t n);
void quick_sort_fat_one_branch_cutoff40(int* arr, size_t n);

#endif
