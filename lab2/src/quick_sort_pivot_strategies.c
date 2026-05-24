#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../include/point5_sorts.h"

static const ptrdiff_t MEDIAN_GROUP_SIZE = 5;
static const ptrdiff_t MIN_SORT_SIZE = 2;
static const ptrdiff_t MIDDLE_DIVISOR = 2;
static const ptrdiff_t RANDOM_INDEX_OFFSET = 1;
static const ptrdiff_t QUICK_SORT_CUTOFF = 40;

typedef int (*pivot_selector_t)(int* a, ptrdiff_t l, ptrdiff_t r);

typedef struct {
    ptrdiff_t equals_from;
    ptrdiff_t equals_to;
} fat_partition_result_t;

void lab2_swap_int(int* a, int* b);
int lab2_median3_values(int x, int y, int z);

static ptrdiff_t middle_index(ptrdiff_t l, ptrdiff_t r) {
    return l + (r - l) / MIDDLE_DIVISOR;
}

static ptrdiff_t min_ptrdiff(ptrdiff_t x, ptrdiff_t y) {
    return (x < y) ? x : y;
}

static void ensure_random_seeded(void) {
    static int seeded = 0;

    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }
}

static ptrdiff_t random_index(ptrdiff_t l, ptrdiff_t r) {
    ensure_random_seeded();
    return l + (ptrdiff_t)(rand() % (int)(r - l + RANDOM_INDEX_OFFSET));
}


static void insertion_sort_range(int* a, ptrdiff_t l, ptrdiff_t r) {
    for (ptrdiff_t i = l + 1; i <= r; i++) {
        int value = a[i];
        ptrdiff_t j = i;

        while (j > l && a[j - 1] > value) {
            a[j] = a[j - 1];
            j--;
        }

        a[j] = value;
    }
}


static fat_partition_result_t fat_partition_by_value(int* a, ptrdiff_t l, ptrdiff_t r, int pivot) {
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


static int pivot_middle(int* a, ptrdiff_t l, ptrdiff_t r) {
    return a[middle_index(l, r)];
}

static int pivot_median3(int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t m = middle_index(l, r);
    return lab2_median3_values(a[l], a[m], a[r]);
}

static int pivot_random(int* a, ptrdiff_t l, ptrdiff_t r) {
    return a[random_index(l, r)];
}

static int pivot_median3_random(int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t i1 = random_index(l, r);
    ptrdiff_t i2 = random_index(l, r);
    ptrdiff_t i3 = random_index(l, r);

    return lab2_median3_values(a[i1], a[i2], a[i3]);
}


static int select_kth_median_of_medians(int* a, ptrdiff_t l, ptrdiff_t r, ptrdiff_t k) {
    while (1) {
        ptrdiff_t n = r - l + 1;

        if (n <= MEDIAN_GROUP_SIZE) {
            insertion_sort_range(a, l, r);
            return a[k];
        }

        ptrdiff_t medians_count = 0;

        for (ptrdiff_t group_start = l; group_start <= r; group_start += MEDIAN_GROUP_SIZE) {
            ptrdiff_t group_end = min_ptrdiff(group_start + MEDIAN_GROUP_SIZE - 1, r);
            insertion_sort_range(a, group_start, group_end);

            ptrdiff_t median_index = group_start + (group_end - group_start) / MIDDLE_DIVISOR;
            lab2_swap_int(&a[l + medians_count], &a[median_index]);
            medians_count++;
        }

        ptrdiff_t med_l = l;
        ptrdiff_t med_r = l + medians_count - 1;
        ptrdiff_t med_k = med_l + (med_r - med_l) / MIDDLE_DIVISOR;

        int pivot = select_kth_median_of_medians(a, med_l, med_r, med_k);
        fat_partition_result_t part = fat_partition_by_value(a, l, r, pivot);

        if (k < part.equals_from) {
            r = part.equals_from - 1;
        } else if (k > part.equals_to) {
            l = part.equals_to + 1;
        } else {
            return pivot;
        }
    }
}

static int pivot_median_of_medians(int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t k = l + (r - l) / MIDDLE_DIVISOR;
    return select_kth_median_of_medians(a, l, r, k);
}


static void quick_sort_impl(
    int* a,
    ptrdiff_t l,
    ptrdiff_t r,
    pivot_selector_t select_pivot
) {
    while (l < r) {
        if (r - l + 1 <= QUICK_SORT_CUTOFF) {
            insertion_sort_range(a, l, r);
            return;
        }

        int pivot = select_pivot(a, l, r);
        fat_partition_result_t part = fat_partition_by_value(a, l, r, pivot);

        ptrdiff_t left_l = l;
        ptrdiff_t left_r = part.equals_from - 1;
        ptrdiff_t right_l = part.equals_to + 1;
        ptrdiff_t right_r = r;

        ptrdiff_t left_size = (left_l <= left_r) ? (left_r - left_l + 1) : 0;
        ptrdiff_t right_size = (right_l <= right_r) ? (right_r - right_l + 1) : 0;

        if (left_size < right_size) {
            if (left_l < left_r) {
                quick_sort_impl(a, left_l, left_r, select_pivot);
            }
            l = right_l;
        } else {
            if (right_l < right_r) {
                quick_sort_impl(a, right_l, right_r, select_pivot);
            }
            r = left_r;
        }
    }
}


void quick_sort_pivot_middle(int* arr, size_t n) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_impl(arr, 0, (ptrdiff_t)n - 1, pivot_middle);
}

void quick_sort_pivot_median3(int* arr, size_t n) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_impl(arr, 0, (ptrdiff_t)n - 1, pivot_median3);
}

void quick_sort_pivot_random(int* arr, size_t n) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_impl(arr, 0, (ptrdiff_t)n - 1, pivot_random);
}

void quick_sort_pivot_median3_random(int* arr, size_t n) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_impl(arr, 0, (ptrdiff_t)n - 1, pivot_median3_random);
}

void quick_sort_pivot_median_of_medians(int* arr, size_t n) {
    if ((ptrdiff_t)n < MIN_SORT_SIZE) {
        return;
    }

    quick_sort_impl(arr, 0, (ptrdiff_t)n - 1, pivot_median_of_medians);
}
