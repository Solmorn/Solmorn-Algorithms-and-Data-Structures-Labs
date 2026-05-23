#include <stdlib.h>
#include <string.h>

#include "list_stack.h"

ListStack* list_stack_ctr(size_t elem_size) {
    if (elem_size == 0) return NULL;

    ListStack* st = (ListStack*)calloc(1, sizeof(ListStack));
    if (!st) return NULL;

    st->head      = NULL;
    st->elem_size = elem_size;

    return st;
}

int list_stack_push(ListStack* st, const void* buffer) {
    if (!st || !buffer) return 0;

    Node* n = (Node*)calloc(1, sizeof(Node));
    if (!n) return 0;

    n->value = calloc(1, st->elem_size);
    if (!n->value) {
        free(n);
        return 0;
    }

    memcpy(n->value, buffer, st->elem_size);
    n->next  = st->head;
    st->head = n;

    return 1;
}

int list_stack_top(ListStack* st, void* buffer) {
    if (!st || !buffer || !st->head) return 0;

    memcpy(buffer, st->head->value, st->elem_size);

    return 1;
}

int list_stack_pop(ListStack* st) {
    if (!st || !st->head) return 0;

    Node* n = st->head;
    st->head = n->next;

    free(n->value);
    free(n);

    return 1;
}

ListStack* list_stack_dtr(ListStack* st) {
    if (!st) return NULL;

    while (st->head) {
        Node* n = st->head;
        st->head = n->next;

        free(n->value);
        free(n);
    }

    free(st);

    return NULL;
}
