#include <stddef.h>

#include "../include/point4_sorts.h"

typedef struct {
    ptrdiff_t equals_from;
    ptrdiff_t equals_to;
} fat_partition_result_t;

static void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


static ptrdiff_t middle_index(ptrdiff_t left, ptrdiff_t right) {
    return left + (right - left) / 2;
}



static ptrdiff_t lomuto_partition(int* a, ptrdiff_t l, ptrdiff_t r) {
    int pivot = a[middle_index(l, r)];
    swap_int(&a[r], &a[middle_index(l, r)]);

    ptrdiff_t i = l;
    for (ptrdiff_t j = l; j <= r; ++j) {
        if (a[j] < pivot) {
            swap_int(&a[i++], &a[j]);
        }
    }

    swap_int(&a[i], &a[r]);
    return i;
}

static void quick_sort_lomuto_impl(int* a, ptrdiff_t l, ptrdiff_t r) {
    if (l >= r) {
        return;
    }

    ptrdiff_t pivot_pos = lomuto_partition(a, l, r);

    quick_sort_lomuto_impl(a, l, pivot_pos - 1);
    quick_sort_lomuto_impl(a, pivot_pos + 1, r);
}

void quick_sort_lomuto(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    quick_sort_lomuto_impl(arr, 0, (ptrdiff_t)n - 1);
}


static ptrdiff_t hoare_partition(int* a, ptrdiff_t l, ptrdiff_t r) {
    ptrdiff_t piv_idx = middle_index(l, r);
    int pivot = a[piv_idx];

    ptrdiff_t i = l;
    ptrdiff_t j = r;

    while (i <= j) {
        while (a[i] < pivot) {
            ++i;
        }

        while (a[j] > pivot) {
            --j;
        }

        if (i >= j) {
            return j;
        }

        swap_int(&a[i++], &a[j--]);
    }

    return j;
}

static void quick_sort_hoare_impl(int* a, ptrdiff_t l, ptrdiff_t r) {
    if (l >= r) {
        return;
    }

    ptrdiff_t split = hoare_partition(a, l, r);

    quick_sort_hoare_impl(a, l, split);
    quick_sort_hoare_impl(a, split + 1, r);
}

void quick_sort_hoare(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    quick_sort_hoare_impl(arr, 0, (ptrdiff_t)n - 1);
}


static fat_partition_result_t fat_partition(int* a, ptrdiff_t l, ptrdiff_t r) {
    int pivot = a[middle_index(l, r)];
    ptrdiff_t mid = l;
    ptrdiff_t left = l;
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

static void quick_sort_fat_impl(int* a, ptrdiff_t l, ptrdiff_t r) {
    if (l >= r) {
        return;
    }

    fat_partition_result_t part = fat_partition(a, l, r);

    quick_sort_fat_impl(a, l, part.equals_from - 1);
    quick_sort_fat_impl(a, part.equals_to + 1, r);
}

void quick_sort_fat(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    quick_sort_fat_impl(arr, 0, (ptrdiff_t)n - 1);
}


static void quick_sort_fat_one_branch_impl(int* a, ptrdiff_t l, ptrdiff_t r) {
    while (l < r) {
        fat_partition_result_t part = fat_partition(a, l, r);

        ptrdiff_t left_l = l;
        ptrdiff_t left_r = part.equals_from - 1;
        ptrdiff_t right_l = part.equals_to + 1;
        ptrdiff_t right_r = r;

        ptrdiff_t left_size = (left_l <= left_r) ? (left_r - left_l + 1) : 0;
        ptrdiff_t right_size = (right_l <= right_r) ? (right_r - right_l + 1) : 0;

        if (left_size < right_size) {
            if (left_l < left_r) {
                quick_sort_fat_one_branch_impl(a, left_l, left_r);
            }
            l = right_l;
        } else {
            if (right_l < right_r) {
                quick_sort_fat_one_branch_impl(a, right_l, right_r);
            }
            r = left_r;
        }
    }
}

void quick_sort_fat_one_branch(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    quick_sort_fat_one_branch_impl(arr, 0, (ptrdiff_t)n - 1);
}


static void insertion_sort_range(int* a, ptrdiff_t l, ptrdiff_t r) {
    for (ptrdiff_t i = l + 1; i <= r; ++i) {
        int value = a[i];
        ptrdiff_t j = i;

        while (j > l && a[j - 1] > value) {
            a[j] = a[j - 1];
            --j;
        }

        a[j] = value;
    }
}

static void quick_sort_fat_one_branch_cutoff40_impl(int* a, ptrdiff_t l, ptrdiff_t r) {
    const ptrdiff_t cutoff = 40;

    while (l < r) {
        if (r - l + 1 <= cutoff) {
            insertion_sort_range(a, l, r);
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
                quick_sort_fat_one_branch_cutoff40_impl(a, left_l, left_r);
            }
            l = right_l;
        } else {
            if (right_l < right_r) {
                quick_sort_fat_one_branch_cutoff40_impl(a, right_l, right_r);
            }
            r = left_r;
        }
    }
}

void quick_sort_fat_one_branch_cutoff40(int* arr, size_t n) {
    if (n < 2) {
        return;
    }

    quick_sort_fat_one_branch_cutoff40_impl(arr, 0, (ptrdiff_t)n - 1);
}