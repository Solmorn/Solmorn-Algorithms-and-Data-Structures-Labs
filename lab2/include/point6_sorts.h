#ifndef POINT6_SORTS_H
#define POINT6_SORTS_H

#include <stddef.h>

void quick_sort_shell_cutoff_t8(int* arr, size_t n);
void quick_sort_shell_cutoff_t16(int* arr, size_t n);
void quick_sort_shell_cutoff_t24(int* arr, size_t n);
void quick_sort_shell_cutoff_t32(int* arr, size_t n);
void quick_sort_shell_cutoff_t40(int* arr, size_t n);
void quick_sort_shell_cutoff_t48(int* arr, size_t n);
void quick_sort_shell_cutoff_t64(int* arr, size_t n);

#endif