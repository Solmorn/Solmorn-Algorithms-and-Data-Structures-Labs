#include <stddef.h>

#include "../include/point6_sorts.h"

static const ptrdiff_t MIN_SORT_SIZE = 2;
static const ptrdiff_t MIDDLE_DIVISOR = 2;
static const ptrdiff_t SHELL_STEP_DIVISOR = 9;
static const ptrdiff_t SHELL_STEP_MULTIPLIER = 3;
static const ptrdiff_t SHELL_STEP_ADDEND = 1;
static const ptrdiff_t CUTOFF_T8 = 8;
static const ptrdiff_t CUTOFF_T16 = 16;
static const ptrdiff_t CUTOFF_T24 = 24;
static const ptrdiff_t CUTOFF_T32 = 32;
static const ptrdiff_t CUTOFF_T40 = 40;
static const ptrdiff_t CUTOFF_T48 = 48;
static const ptrdiff_t CUTOFF_T64 = 64;

typedef struct {
    ptrdiff_t equals_from;
    ptrdiff_t equals_to;
} fat_partition_result_t;

void lab2_swap_int(int* a, int* b);
int lab2_median3_values(int x, int y, int z);

static ptrdiff_t middle_index(ptrdiff_t l, ptrdiff_t r) {
    return l + (r - l) / MIDDLE_DIVISOR;
}

static int select_best_pivot(const int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t m = middle_index(l, r);
    return lab2_median3_values(a[l], a[m], a[r]);
}

static fat_partition_result_t fat_partition(int* a, ptrdiff_t l, ptrdiff_t r) {
    int pivot = select_best_pivot(a, l, r);
    ptrdiff_t left = l;
    ptrdiff_t mid = l;
    ptrdiff_t right = r;

    while (mid <= right) {
        if (a[mid] < pivot) {
            lab2_swap_int(&a[left++], &a[mid++]);
        } else if (a[mid] == pivot) {
            mid++;
        } else {
            lab2_swap_int(&a[mid], &a[right--]);
        }
    }

    fat_partition_result_t result;
    result.equals_from = left;
    result.equals_to = right;
    return result;
}

static void shell_sort_range(int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t n = r - l + 1;

    if (n < MIN_SORT_SIZE) {
        return;
    }

    ptrdiff_t step = 1;
    while (step <= n / SHELL_STEP_DIVISOR) {
        step = step * SHELL_STEP_MULTIPLIER + SHELL_STEP_ADDEND;
    }

    for (; step > 0; step /= SHELL_STEP_MULTIPLIER) {
        for (ptrdiff_t i = l + step; i <= r; i++) {
            int value = a[i];
            ptrdiff_t j = i;

            while (j >= l + step && a[j - step] > value) {
                a[j] = a[j - step];
                j -= step;
            }

            a[j] = value;
        }

        if (step == 1) {
            break;
        }
    }
}

static void quick_sort_shell_cutoff_impl(int* a, ptrdiff_t l, ptrdiff_t r, ptrdiff_t cutoff) {
    while (l < r) {
        if (r - l + 1 <= cutoff) {
            shell_sort_range(a, l, r);
            return;
        }

        fat_partition_result_t part = fat_partition(a, l, r);

        ptrdiff_t left_l = l;
        ptrdiff_t left_r = part.equals_from - 1;
        ptrdiff_t right_l = part.equals_to + 1;
        ptrdiff_t right_r = r;

        ptrdiff_t left_size = (left_l <= left_r) ? (left_r - left_l + 1) : 0;
        ptrdiff_t right_size = (right_l <= right_r) ? (right_r - right_l + 1) : 0;

        if (left_size < right_size) {
            if (left_l < left_r) {
                quick_sort_shell_cutoff_impl(a, left_l, left_r, cutoff);
            }
            l = right_l;
        } else {
            if (right_l < right_r) {
                quick_sort_shell_cutoff_impl(a, right_l, right_r, cutoff);
            }
            r = left_r;
        }
    }
}

static void quick_sort_shell_cutoff(int* arr, size_t n, ptrdiff_t cutoff) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_shell_cutoff_impl(arr, 0, (ptrdiff_t)n - 1, cutoff);
}

void quick_sort_shell_cutoff_t8(int* arr, size_t n)  { quick_sort_shell_cutoff(arr, n, CUTOFF_T8); }
void quick_sort_shell_cutoff_t16(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, CUTOFF_T16); }
void quick_sort_shell_cutoff_t24(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, CUTOFF_T24); }
void quick_sort_shell_cutoff_t32(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, CUTOFF_T32); }
void quick_sort_shell_cutoff_t40(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, CUTOFF_T40); }
void quick_sort_shell_cutoff_t48(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, CUTOFF_T48); }
void quick_sort_shell_cutoff_t64(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, CUTOFF_T64); }
