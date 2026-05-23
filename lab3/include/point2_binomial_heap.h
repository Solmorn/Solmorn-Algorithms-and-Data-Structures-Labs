#ifndef POINT2_BINOMIAL_HEAP_H
#define POINT2_BINOMIAL_HEAP_H

#include <assert.h>
#include <stdlib.h>

typedef struct HeapElem {
    long long value;
    int request_index;
    int degree;
    struct HeapElem* parent;
    struct HeapElem* child;
    struct HeapElem* sibling;
} HeapElem;

typedef struct {
    HeapElem* head;
    HeapElem** positions;
} Heap;

static int less(HeapElem* a, HeapElem* b) {
    if (a->value != b->value) return a->value < b->value;
    return a->request_index < b->request_index;
}

static HeapElem* heap_elem_ctor(long long value, int request_index) {
    HeapElem* elem = (HeapElem*)calloc(1, sizeof(HeapElem));
    elem->value = value;
    elem->request_index = request_index;
    return elem;
}

static Heap* heap_ctor(void) {
    Heap* heap = (Heap*)calloc(1, sizeof(Heap));
    assert(heap != NULL);

    heap->head = NULL;
    heap->positions = NULL;
    return heap;
}

static Heap* heap_ctor_with_positions(HeapElem** positions) {
    Heap* heap = (Heap*)calloc(1, sizeof(Heap));
    assert(heap != NULL);

    heap->head = NULL;
    heap->positions = positions;
    return heap;
}

static void heap_elem_dtor(HeapElem* elem) {
    if (elem == NULL) return;
    heap_elem_dtor(elem->child);
    heap_elem_dtor(elem->sibling);
    free(elem);
}

static void heap_dtor(Heap* heap) {
    if (heap == NULL) return;
    heap_elem_dtor(heap->head);
    free(heap);
}

static void swap_payload(HeapElem* a, HeapElem* b, HeapElem** positions) {
    long long tmp_value = a->value;
    int tmp_index = a->request_index;

    a->value = b->value;
    a->request_index = b->request_index;
    b->value = tmp_value;
    b->request_index = tmp_index;

    if (positions != NULL) {
        positions[a->request_index] = a;
        positions[b->request_index] = b;
    }
}

static void sift_up_node(HeapElem* elem, HeapElem** positions) {
    while (elem->parent != NULL) {
        if (less(elem, elem->parent)) {
            swap_payload(elem, elem->parent, positions);
            elem = elem->parent;
        } else {
            return;
        }
    }
}

static void link_trees(HeapElem* child, HeapElem* parent) {
    child->parent = parent;
    child->sibling = parent->child;
    parent->child = child;
    parent->degree++;
}

static HeapElem* merge_root_lists(HeapElem* a, HeapElem* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;

    HeapElem* head = NULL;
    HeapElem* tail = NULL;

    if (a->degree <= b->degree) {
        head = a;
        a = a->sibling;
    } else {
        head = b;
        b = b->sibling;
    }

    tail = head;

    while (a != NULL && b != NULL) {
        if (a->degree <= b->degree) {
            tail->sibling = a;
            a = a->sibling;
        } else {
            tail->sibling = b;
            b = b->sibling;
        }
        tail = tail->sibling;
    }

    tail->sibling = (a != NULL) ? a : b;
    return head;
}

static void heap_merge(Heap* from, Heap* to) {
    assert(from);
    assert(to);
    if (from == to || from->head == NULL) return;

    to->head = merge_root_lists(to->head, from->head);
    from->head = NULL;
    if (to->head == NULL) return;

    HeapElem* prev = NULL;
    HeapElem* cur = to->head;
    HeapElem* next = cur->sibling;

    while (next != NULL) {
        if (cur->degree != next->degree || (next->sibling != NULL && next->sibling->degree == cur->degree)) {
            prev = cur;
            cur = next;
        } else if (less(cur, next)) {
            cur->sibling = next->sibling;
            link_trees(next, cur);
        } else {
            if (prev == NULL) {
                to->head = next;
            } else {
                prev->sibling = next;
            }
            link_trees(cur, next);
            cur = next;
        }
        next = cur->sibling;
    }
}

static HeapElem* heap_insert_node(Heap* heap, long long value, int request_index) {
    assert(heap);
    HeapElem* elem = heap_elem_ctor(value, request_index);

    if (heap->positions != NULL) {
        heap->positions[request_index] = elem;
    }

    Heap temp = {0};
    temp.head = elem;
    heap_merge(&temp, heap);
    return elem;
}

static void heap_insert(Heap* heap, long long value, int request_index) {
    (void)heap_insert_node(heap, value, request_index);
}

static int heap_empty(Heap* heap) {
    assert(heap);
    return heap->head == NULL;
}

static HeapElem* heap_get_min_node(Heap* heap, HeapElem** prev_min) {
    assert(heap);
    assert(heap->head != NULL);

    HeapElem* cur = heap->head;
    HeapElem* prev = NULL;
    HeapElem* min_node = cur;
    HeapElem* min_prev = NULL;

    while (cur != NULL) {
        if (less(cur, min_node)) {
            min_node = cur;
            min_prev = prev;
        }
        prev = cur;
        cur = cur->sibling;
    }

    *prev_min = min_prev;
    return min_node;
}

static long long heap_get_min(Heap* heap) {
    HeapElem* prev_min = NULL;
    HeapElem* min_node = heap_get_min_node(heap, &prev_min);
    return min_node->value;
}

static void heap_decrease_key_to(Heap* heap, int request_index, long long new_value) {
    assert(heap);
    assert(heap->positions != NULL);

    HeapElem* elem = heap->positions[request_index];
    assert(elem != NULL);
    assert(new_value <= elem->value);

    elem->value = new_value;
    sift_up_node(elem, heap->positions);
}

static HeapElem* reverse_children(HeapElem* child) {
    HeapElem* reversed = NULL;

    while (child != NULL) {
        HeapElem* next = child->sibling;
        child->parent = NULL;
        child->sibling = reversed;
        reversed = child;
        child = next;
    }

    return reversed;
}

static HeapElem* heap_extract_min_node(Heap* heap) {
    assert(heap);
    assert(heap->head != NULL);

    HeapElem* prev_min = NULL;
    HeapElem* min_node = heap_get_min_node(heap, &prev_min);

    if (prev_min == NULL) {
        heap->head = min_node->sibling;
    } else {
        prev_min->sibling = min_node->sibling;
    }

    HeapElem* reversed = reverse_children(min_node->child);
    Heap temp = {0};
    temp.head = reversed;
    heap_merge(&temp, heap);

    min_node->parent = NULL;
    min_node->child = NULL;
    min_node->sibling = NULL;
    min_node->degree = 0;

    if (heap->positions != NULL) {
        heap->positions[min_node->request_index] = NULL;
    }

    return min_node;
}

static int heap_check_subtree(HeapElem* node, HeapElem* parent) {
    while (node != NULL) {
        if (node->parent != parent) return 0;
        if (parent != NULL && less(node, parent)) return 0;

        int child_count = 0;
        HeapElem* child = node->child;
        while (child != NULL) {
            child_count++;
            child = child->sibling;
        }

        if (child_count != node->degree) return 0;
        if (!heap_check_subtree(node->child, node)) return 0;

        node = node->sibling;
    }
    return 1;
}

static int heap_is_valid(Heap* heap) {
    assert(heap);

    int prev_degree = -1;
    HeapElem* cur = heap->head;

    while (cur != NULL) {
        if (cur->parent != NULL) return 0;
        if (cur->degree <= prev_degree) return 0;
        if (!heap_check_subtree(cur->child, cur)) return 0;

        prev_degree = cur->degree;
        cur = cur->sibling;
    }

    return 1;
}

#endif
