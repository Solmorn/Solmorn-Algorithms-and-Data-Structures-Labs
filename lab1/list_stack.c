#include <stdlib.h>

#include "list_stack.h"

ListStack* list_stack_ctr() {

    ListStack *st = (ListStack*)calloc(1, sizeof(ListStack));
    if (!st) return NULL;

    st->head = NULL;
    return st;
}

int list_stack_push(ListStack* st, int* buffer) {

    Node *n = (Node*)calloc(1, sizeof(Node));
    if (!n) return 0;

    n->value = *buffer;
    n->next = st->head;
    st->head = n;

    return 1;
}

int list_stack_top(ListStack* st, int* buffer) {
    
    if (!st->head) return 0;

    *buffer = st->head->value;
    return 1;
}

int list_stack_pop(ListStack* st) {
    if (!st->head) return 0;

    Node *n = st->head;
    st->head = n->next;
    free(n);

    return 1;
}

ListStack* list_stack_dtr(ListStack* st) {

    while (st && st->head) {
        Node* n = st->head;
        st->head = n->next;
        free(n);
    }

    free(st);

    return NULL;
}