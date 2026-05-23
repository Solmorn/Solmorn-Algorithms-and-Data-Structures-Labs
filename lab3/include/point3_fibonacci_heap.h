#ifndef POINT3_FIBONACCI_HEAP_H
#define POINT3_FIBONACCI_HEAP_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define FIB_MAX_DEGREE 64

typedef struct FibNode {
    long long key;
    int vertex;
    int degree;
    int mark;
    struct FibNode* parent;
    struct FibNode* child;
    struct FibNode* left;
    struct FibNode* right;
} FibNode;

typedef struct {
    FibNode* min;
    size_t size;
    FibNode** positions;
} FibHeap;

static int fib_less(FibNode* a, FibNode* b) {
    if (a->key != b->key) return a->key < b->key;
    return a->vertex < b->vertex;
}

static FibHeap* fib_heap_ctor(FibNode** positions) {
    FibHeap* heap = (FibHeap*)calloc(1, sizeof(FibHeap));
    assert(heap != NULL);

    heap->min = NULL;
    heap->size = 0;
    heap->positions = positions;
    return heap;
}

static FibNode* fib_node_ctor(long long key, int vertex) {
    FibNode* node = (FibNode*)calloc(1, sizeof(FibNode));
    node->key = key;
    node->vertex = vertex;
    node->left = node;
    node->right = node;
    return node;
}

static void fib_merge_cycles(FibNode* a, FibNode* b) {
    FibNode* a_right = a->right;
    FibNode* b_left = b->left;
    a->right = b;
    b->left = a;
    a_right->left = b_left;
    b_left->right = a_right;
}

static void fib_remove_from_list(FibNode* node) {
    node->left->right = node->right;
    node->right->left = node->left;
    node->left = node;
    node->right = node;
}

static void fib_add_root(FibHeap* heap, FibNode* node) {
    node->parent = NULL;
    node->mark = 0;

    if (heap->min == NULL) {
        node->left = node;
        node->right = node;
        heap->min = node;
    } else {
        fib_merge_cycles(heap->min, node);
        if (fib_less(node, heap->min)) heap->min = node;
    }
}

static void fib_heap_insert(FibHeap* heap, long long key, int vertex) {
    assert(heap);
    FibNode* node = fib_node_ctor(key, vertex);

    if (heap->positions != NULL) {
        heap->positions[vertex] = node;
    }

    fib_add_root(heap, node);
    heap->size++;
}

static int fib_heap_empty(FibHeap* heap) {
    assert(heap);
    return heap->min == NULL;
}

static void fib_link(FibNode* child, FibNode* parent) {
    fib_remove_from_list(child);
    child->parent = parent;
    child->mark = 0;

    if (parent->child == NULL) {
        child->left = child;
        child->right = child;
        parent->child = child;
    } else {
        fib_merge_cycles(parent->child, child);
    }

    parent->degree++;
}

static void fib_consolidate(FibHeap* heap) {
    FibNode* by_degree[FIB_MAX_DEGREE];
    memset(by_degree, 0, sizeof(by_degree));

    FibNode** roots = (FibNode**)calloc(heap->size + 1, sizeof(FibNode*));
    assert(roots != NULL);

    size_t root_count = 0;
    FibNode* cur = heap->min;

    do {
        roots[root_count++] = cur;
        cur = cur->right;
    } while (cur != heap->min);

    heap->min = NULL;

    for (size_t i = 0; i < root_count; ++i) {
        FibNode* x = roots[i];
        x->left = x;
        x->right = x;
        int degree = x->degree;

        while (by_degree[degree] != NULL) {
            FibNode* y = by_degree[degree];

            if (fib_less(y, x)) {
                FibNode* tmp = x;
                x = y;
                y = tmp;
            }

            by_degree[degree] = NULL;
            fib_link(y, x);
            degree++;
            assert(degree < FIB_MAX_DEGREE);
        }

        by_degree[degree] = x;
    }

    for (int degree = 0; degree < FIB_MAX_DEGREE; ++degree) {
        if (by_degree[degree] == NULL) continue;

        if (heap->min == NULL) {
            heap->min = by_degree[degree];
            heap->min->left = heap->min;
            heap->min->right = heap->min;
        } else {
            fib_merge_cycles(heap->min, by_degree[degree]);
            if (fib_less(by_degree[degree], heap->min)) heap->min = by_degree[degree];
        }
    }

    free(roots);
}

static void fib_cut(FibHeap* heap, FibNode* node, FibNode* parent) {
    if (node->right == node) {
        parent->child = NULL;
    } else {
        if (parent->child == node) parent->child = node->right;
        node->left->right = node->right;
        node->right->left = node->left;
    }

    parent->degree--;
    node->left = node;
    node->right = node;
    node->parent = NULL;
    node->mark = 0;
    fib_add_root(heap, node);
}

static void fib_cascading_cut(FibHeap* heap, FibNode* node) {
    FibNode* parent = node->parent;
    if (parent == NULL) return;

    if (!node->mark) {
        node->mark = 1;
        return;
    }

    fib_cut(heap, node, parent);
    fib_cascading_cut(heap, parent);
}

static void fib_heap_decrease_key_to(FibHeap* heap, int vertex, long long new_key) {
    assert(heap);
    assert(heap->positions != NULL);

    FibNode* node = heap->positions[vertex];
    assert(node != NULL);
    assert(new_key <= node->key);

    node->key = new_key;

    FibNode* parent = node->parent;
    if (parent != NULL && fib_less(node, parent)) {
        fib_cut(heap, node, parent);
        fib_cascading_cut(heap, parent);
    }

    if (fib_less(node, heap->min)) heap->min = node;
}

static FibNode* fib_heap_extract_min_node(FibHeap* heap) {
    assert(heap);
    assert(heap->min != NULL);

    FibNode* min_node = heap->min;

    if (min_node->child != NULL) {
        FibNode* cur = min_node->child;
        do {
            cur->parent = NULL;
            cur->mark = 0;
            cur = cur->right;
        } while (cur != min_node->child);
    }

    if (min_node->right == min_node) {
        heap->min = min_node->child;
    } else {
        FibNode* next_root = min_node->right;
        fib_remove_from_list(min_node);
        heap->min = next_root;

        if (min_node->child != NULL) {
            fib_merge_cycles(heap->min, min_node->child);
        }
    }

    if (heap->positions != NULL) {
        heap->positions[min_node->vertex] = NULL;
    }

    min_node->parent = NULL;
    min_node->child = NULL;
    min_node->left = min_node;
    min_node->right = min_node;
    min_node->degree = 0;
    min_node->mark = 0;

    heap->size--;

    if (heap->min != NULL) {
        fib_consolidate(heap);
    }

    return min_node;
}

static void fib_free_cycle(FibNode* start) {
    if (start == NULL) return;

    FibNode* cur = start->right;

    while (cur != start) {
        FibNode* next = cur->right;
        fib_free_cycle(cur->child);
        free(cur);
        cur = next;
    }

    fib_free_cycle(start->child);
    free(start);
}

static void fib_heap_dtor(FibHeap* heap) {
    if (heap == NULL) return;
    fib_free_cycle(heap->min);
    free(heap);
}

#endif
