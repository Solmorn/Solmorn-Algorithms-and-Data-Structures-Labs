#ifndef POINT8_SORTS_H
#define POINT8_SORTS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void timsort_github(int* arr, size_t n);
void pdqsort_github(int* arr, size_t n);

#ifdef __cplusplus
}
#endif

#endif