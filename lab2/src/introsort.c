#include <stddef.h>
#include <stdlib.h>

#include "../include/point7_sorts.h"

static const ptrdiff_t MIN_SORT_SIZE = 2;
static const ptrdiff_t MIDDLE_DIVISOR = 2;
static const ptrdiff_t SMALL_BLOCK_CUTOFF = 32;
static const ptrdiff_t SHELL_STEP_DIVISOR = 9;
static const ptrdiff_t SHELL_STEP_MULTIPLIER = 3;
static const ptrdiff_t SHELL_STEP_ADDEND = 1;
static const size_t OPTIMAL_HEAP_K = 4u;
static const int INTROSORT_C1 = 1;
static const int INTROSORT_C2 = 2;
static const int INTROSORT_C3 = 3;
static const int INTROSORT_C4 = 4;
static const int BEST_C = 2;

typedef struct {
    ptrdiff_t equals_from;
    ptrdiff_t equals_to;
} fat_partition_result_t;

void lab2_swap_int(int* a, int* b);
int lab2_median3_values(int x, int y, int z);

static ptrdiff_t middle_index(ptrdiff_t l, ptrdiff_t r) {
    return l + (r - l) / MIDDLE_DIVISOR;
}

static int select_pivot_median3(const int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t m = middle_index(l, r);
    return lab2_median3_values(a[l], a[m], a[r]);
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


static fat_partition_result_t fat_partition(int* a, ptrdiff_t l, ptrdiff_t r) {

    int pivot = select_pivot_median3(a, l, r);

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



static int floor_log2_size(size_t n) {

    int result = 0;

    while (n > 1u) {
        n >>= 1u;
        result++;
    }

    return result;
}


static size_t heap_parent(size_t index, size_t k) {
    return (index - 1u) / k;
}

static size_t heap_first_child(size_t index, size_t k) {
    return index * k + 1u;
}

static int heap_has_child(size_t index, size_t heap_size, size_t k) {
    return heap_first_child(index, k) < heap_size;
}

static size_t heap_max_child_index(const int* base, size_t index, size_t heap_size, size_t k) {

    size_t first_child = heap_first_child(index, k);
    size_t best_child = first_child;
    size_t last_exclusive = first_child + k;

    if (last_exclusive > heap_size) {
        last_exclusive = heap_size;
    }

    for (size_t child = first_child + 1u; child < last_exclusive; child++) {
        if (base[child] > base[best_child]) {
            best_child = child;
        }
    }

    return best_child;
}

static void heap_bottom_up_sift_down(int* base, size_t start, size_t heap_size, size_t k) {

    if (!heap_has_child(start, heap_size, k)) {
        return;
    }

    int x = base[start];
    size_t hole = start;

    while (heap_has_child(hole, heap_size, k)) {
        size_t child = heap_max_child_index(base, hole, heap_size, k);
        base[hole] = base[child];
        hole = child;
    }

    while (hole > start) {
        size_t parent = heap_parent(hole, k);

        if (base[parent] >= x) {
            break;
        }

        base[hole] = base[parent];
        hole = parent;
    }

    base[hole] = x;
}

static void heap_build(int* base, size_t n, size_t k) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    size_t last_internal = (n - 2u) / k;

    for (size_t i = last_internal + 1u; i > 0u; i--) {
        heap_bottom_up_sift_down(base, i - 1u, n, k);
    }
}

static void heap_sort_range(int* a, ptrdiff_t l, ptrdiff_t r) {
    size_t n = (size_t)(r - l + 1);
    int* base = a + l;

    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    heap_build(base, n, OPTIMAL_HEAP_K);

    for (size_t heap_size = n; heap_size > 1u; heap_size--) {
        lab2_swap_int(&base[0], &base[heap_size - 1u]);
        heap_bottom_up_sift_down(base, 0u, heap_size - 1u, OPTIMAL_HEAP_K);
    }
}


static void quick_sort_best_impl(int* a, ptrdiff_t l, ptrdiff_t r) {
    while (l < r) {
        if (r - l + 1 <= SMALL_BLOCK_CUTOFF) {
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
                quick_sort_best_impl(a, left_l, left_r);
            }
            l = right_l;
        } else {
            if (right_l < right_r) {
                quick_sort_best_impl(a, right_l, right_r);
            }
            r = left_r;
        }
    }
}

void quick_sort_best_cutoff32(int* arr, size_t n) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_best_impl(arr, 0, (ptrdiff_t)n - 1);
}


static void introsort_impl(int* a, ptrdiff_t l, ptrdiff_t r, int depth_limit) {
    while (l < r) {
        if (r - l + 1 <= SMALL_BLOCK_CUTOFF) {
            shell_sort_range(a, l, r);
            return;
        }

        if (depth_limit <= 0) {
            heap_sort_range(a, l, r);
            return;
        }

        depth_limit--;

        fat_partition_result_t part = fat_partition(a, l, r);

        ptrdiff_t left_l = l;
        ptrdiff_t left_r = part.equals_from - 1;
        ptrdiff_t right_l = part.equals_to + 1;
        ptrdiff_t right_r = r;

        ptrdiff_t left_size = (left_l <= left_r) ? (left_r - left_l + 1) : 0;
        ptrdiff_t right_size = (right_l <= right_r) ? (right_r - right_l + 1) : 0;

        if (left_size < right_size) {
            if (left_l < left_r) {
                introsort_impl(a, left_l, left_r, depth_limit);
            }
            l = right_l;
        } else {
            if (right_l < right_r) {
                introsort_impl(a, right_l, right_r, depth_limit);
            }
            r = left_r;
        }
    }
}

static void introsort_with_c(int* arr, size_t n, int c) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    int depth_limit = c * floor_log2_size(n);
    introsort_impl(arr, 0, (ptrdiff_t)n - 1, depth_limit);
}

void introsort_c1(int* arr, size_t n) {
    introsort_with_c(arr, n, INTROSORT_C1);
}

void introsort_c2(int* arr, size_t n) {
    introsort_with_c(arr, n, INTROSORT_C2);
}

void introsort_c3(int* arr, size_t n) {
    introsort_with_c(arr, n, INTROSORT_C3);
}

void introsort_c4(int* arr, size_t n) {
    introsort_with_c(arr, n, INTROSORT_C4);
}

void introsort_best(int* arr, size_t n) {
    introsort_with_c(arr, n, BEST_C);
}
