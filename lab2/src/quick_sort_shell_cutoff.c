#include <stddef.h>

#include "../include/point6_sorts.h"

typedef struct {
    ptrdiff_t equals_from;
    ptrdiff_t equals_to;
} fat_partition_result_t;

static void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static ptrdiff_t middle_index(ptrdiff_t l, ptrdiff_t r) {
    return l + (r - l) / 2;
}

static int median3_values(int x, int y, int z) {
    if (x > y) {
        int t = x;
        x = y;
        y = t;
    }

    if (y > z) {
        int t = y;
        y = z;
        z = t;
    }

    if (x > y) {
        int t = x;
        x = y;
        y = t;
    }

    return y;
}

static int select_best_pivot(const int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t m = middle_index(l, r);
    return median3_values(a[l], a[m], a[r]);
}

static fat_partition_result_t fat_partition(int* a, ptrdiff_t l, ptrdiff_t r) {
    int pivot = select_best_pivot(a, l, r);
    ptrdiff_t left = l;
    ptrdiff_t mid = l;
    ptrdiff_t right = r;

    while (mid <= right) {
        if (a[mid] < pivot) {
            swap_int(&a[left++], &a[mid++]);
        } else if (a[mid] == pivot) {
            ++mid;
        } else {
            swap_int(&a[mid], &a[right--]);
        }
    }

    fat_partition_result_t result;
    result.equals_from = left;
    result.equals_to = right;
    return result;
}

static void shell_sort_range(int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t n = r - l + 1;

    if (n < 2) {
        return;
    }

    ptrdiff_t step = 1;
    while (step <= n / 9) {
        step = step * 3 + 1;
    }

    for (; step > 0; step /= 3) {
        for (ptrdiff_t i = l + step; i <= r; ++i) {
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
    if (n < 2) {
        return;
    }

    quick_sort_shell_cutoff_impl(arr, 0, (ptrdiff_t)n - 1, cutoff);
}

void quick_sort_shell_cutoff_t8(int* arr, size_t n)  { quick_sort_shell_cutoff(arr, n, 8); }
void quick_sort_shell_cutoff_t16(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, 16); }
void quick_sort_shell_cutoff_t24(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, 24); }
void quick_sort_shell_cutoff_t32(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, 32); }
void quick_sort_shell_cutoff_t40(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, 40); }
void quick_sort_shell_cutoff_t48(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, 48); }
void quick_sort_shell_cutoff_t64(int* arr, size_t n) { quick_sort_shell_cutoff(arr, n, 64); }