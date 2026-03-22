#include "../include/point2_sorts.h"

static void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static size_t parent_index(size_t index, size_t k) {
    return (index - 1) / k;
}

static size_t first_child_index(size_t index, size_t k) {
    return index * k + 1;
}

static int has_child(size_t index, size_t heap_size, size_t k) {
    return first_child_index(index, k) < heap_size;
}

static size_t max_child_index(const int* arr, size_t index, size_t heap_size, size_t k) {
    size_t first_child = first_child_index(index, k);
    size_t best_child = first_child;
    size_t last_exclusive = first_child + k;

    if (last_exclusive > heap_size) {
        last_exclusive = heap_size;
    }

    for (size_t child = first_child + 1; child < last_exclusive; ++child) {
        if (arr[child] > arr[best_child]) {
            best_child = child;
        }
    }

    return best_child;
}


static void bottom_up_sift_down(int* arr, size_t start, size_t heap_size, size_t k) {
    if (!has_child(start, heap_size, k)) {
        return;
    }

    int x = arr[start];
    size_t hole = start;

    while (has_child(hole, heap_size, k)) {
        size_t child = max_child_index(arr, hole, heap_size, k);
        arr[hole] = arr[child];
        hole = child;
    }

    while (hole > start) {
        size_t parent = parent_index(hole, k);

        if (arr[parent] >= x) {
            break;
        }

        arr[hole] = arr[parent];
        hole = parent;
    }

    arr[hole] = x;
}

static void build_k_ary_heap(int* arr, size_t n, size_t k) {
    if (n < 2) {
        return;
    }

    size_t last_internal = (n - 2) / k;

    for (size_t i = last_internal + 1; i > 0; --i) {
        bottom_up_sift_down(arr, i - 1, n, k);
    }
}

static void k_ary_bottom_up_heap_sort(int* arr, size_t n, size_t k) {
    if (n < 2) {
        return;
    }

    build_k_ary_heap(arr, n, k);

    for (size_t heap_size = n; heap_size > 1; --heap_size) {
        swap_int(&arr[0], &arr[heap_size - 1]);
        bottom_up_sift_down(arr, 0, heap_size - 1, k);
    }
}

void heap_sort_k2(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 2); }
void heap_sort_k3(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 3); }
void heap_sort_k4(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 4); }
void heap_sort_k5(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 5); }
void heap_sort_k6(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 6); }
void heap_sort_k7(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 7); }
void heap_sort_k8(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 8); }
void heap_sort_k9(int* arr, size_t n)  { k_ary_bottom_up_heap_sort(arr, n, 9); }
void heap_sort_k10(int* arr, size_t n) { k_ary_bottom_up_heap_sort(arr, n, 10); }