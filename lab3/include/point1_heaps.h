#ifndef POINT1_HEAPS_H
#define POINT1_HEAPS_H

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    long long value;
    int request_index;
} HeapElem;

typedef struct {
    HeapElem* data;
    size_t size;
    size_t cap;
    int* positions;
} Heap;

static void swap(Heap* heap, size_t i, size_t j) {
    assert(heap);

    HeapElem tmp = heap->data[i];
    heap->data[i] = heap->data[j];
    heap->data[j] = tmp;

    if (heap->positions != NULL) {
        heap->positions[heap->data[i].request_index] = (int)i;
        heap->positions[heap->data[j].request_index] = (int)j;
    }
}

static void sift_up(Heap* heap, size_t index) {
    assert(heap);

    while (index != 0) {
        size_t parent = (index - 1) / 2;

        if (heap->data[parent].value > heap->data[index].value) {
            swap(heap, parent, index);
            index = parent;
        } else {
            return;
        }
    }
}

static void sift_down(Heap* heap, size_t index) {
    assert(heap);

    while (1) {
        size_t left_child = index * 2 + 1;
        size_t right_child = index * 2 + 2;
        size_t i_min = index;

        if (left_child < heap->size && heap->data[left_child].value < heap->data[i_min].value) {
            i_min = left_child;
        }

        if (right_child < heap->size && heap->data[right_child].value < heap->data[i_min].value) {
            i_min = right_child;
        }

        if (i_min == index) return;

        swap(heap, index, i_min);
        index = i_min;
    }
}

static Heap* heap_ctor(size_t cap, int* positions) {
    Heap* heap = (Heap*)calloc(1, sizeof(Heap));
    heap->data = (HeapElem*)calloc(cap, sizeof(HeapElem));
    heap->size = 0;
    heap->cap = cap;
    heap->positions = positions;
    return heap;
}

static Heap* heap_view_ctor(HeapElem* data, size_t size) {
    Heap* heap = (Heap*)calloc(1, sizeof(Heap));
    heap->data = data;
    heap->size = size;
    heap->cap = size;
    heap->positions = NULL;
    return heap;
}

static void heap_dtor(Heap* heap) {
    free(heap->data);
    free(heap);
}

static void heap_view_dtor(Heap* heap) {
    free(heap);
}

static void heap_insert(Heap* heap, long long value, int request_index) {
    assert(heap);

    if (heap->size == heap->cap) {
        heap->cap *= 2;
        heap->data = (HeapElem*)realloc(heap->data, heap->cap * sizeof(HeapElem));
    }

    heap->data[heap->size].value = value;
    heap->data[heap->size].request_index = request_index;

    if (heap->positions != NULL) {
        heap->positions[request_index] = (int)heap->size;
    }

    heap->size++;
    sift_up(heap, heap->size - 1);
}

static long long heap_get_min(Heap* heap) {
    assert(heap);
    assert(heap->size > 0);

    return heap->data[0].value;
}

static void heap_extract_min(Heap* heap) {
    assert(heap);
    assert(heap->size > 0);

    swap(heap, 0, heap->size - 1);

    if (heap->positions != NULL) {
        heap->positions[heap->data[heap->size - 1].request_index] = -1;
    }

    heap->size--;

    if (heap->size != 0) {
        sift_down(heap, 0);
    }
}

static void heap_decrease_key(Heap* heap, int request_index, long long delta) {
    assert(heap);
    assert(heap->positions != NULL);

    size_t index = (size_t)heap->positions[request_index];
    heap->data[index].value -= delta;
    sift_up(heap, index);
}

static void heap_build_linear(Heap* heap) {
    assert(heap);

    if (heap->size <= 1) return;

    for (size_t i = heap->size / 2; i > 0; --i) {
        sift_down(heap, i - 1);
    }
}

static void heap_build_by_inserts(Heap* heap) {
    assert(heap);

    size_t n = heap->size;
    heap->size = 0;

    for (size_t i = 0; i < n; ++i) {
        heap->size++;
        sift_up(heap, i);
    }
}

static int heap_is_valid(const Heap* heap) {
    assert(heap);

    for (size_t i = 0; i < heap->size; ++i) {
        size_t left_child = i * 2 + 1;
        size_t right_child = i * 2 + 2;

        if (left_child < heap->size && heap->data[left_child].value < heap->data[i].value) return 0;
        if (right_child < heap->size && heap->data[right_child].value < heap->data[i].value) return 0;
    }

    return 1;
}

static int heap_empty(Heap* heap) {
    assert(heap);
    return heap->size == 0;
}

static void heap_decrease_key_to(Heap* heap, int request_index, long long new_value) {
    assert(heap);
    assert(heap->positions != NULL);

    size_t index = (size_t)heap->positions[request_index];
    assert(index < heap->size);
    assert(new_value <= heap->data[index].value);

    heap->data[index].value = new_value;
    sift_up(heap, index);
}

static HeapElem heap_extract_min_elem(Heap* heap) {
    assert(heap);
    assert(heap->size > 0);

    HeapElem min_elem = heap->data[0];

    swap(heap, 0, heap->size - 1);

    if (heap->positions != NULL) {
        heap->positions[heap->data[heap->size - 1].request_index] = -1;
    }

    heap->size--;

    if (heap->size != 0) {
        sift_down(heap, 0);
    }

    return min_elem;
}

#endif
