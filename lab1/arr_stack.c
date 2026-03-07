#include <stdlib.h>
#include <assert.h>

#include "arr_stack.h"

ArrStack* arr_stack_ctr(size_t size) {

    ArrStack *st = (ArrStack*)calloc(1, sizeof(ArrStack));
    if (!st) return NULL;

    st->data = (int*)calloc(1, sizeof(int) * size);
    if (!st->data) { 
        free(st); 
        return NULL; 
    }

    st->size = 0;
    st->cap  = size;

    return st;
}

int arr_stack_push(ArrStack* st, int* buffer) {

    if (st->size == st->cap) {
        size_t new_cap = (st->cap == 0) ? 1 : st->cap * 2;
        int *new_data = (int*)realloc(st->data, sizeof(int) * new_cap);

        if (!new_data) return 0;

        st->data = new_data;
        st->cap  = new_cap;
    }

    st->data[st->size] = *buffer;
    st->size++;

    return 1;
}

int arr_stack_top(ArrStack* st, int* buffer) {

    if (st->size == 0) return 0;

    *buffer = st->data[st->size - 1];

    return 1;
}

int arr_stack_pop(ArrStack* st) {

    if (st->size == 0) return 0;

    st->size--;

    if (st->cap > 1 && st->size * 4 < st->cap) {

        size_t new_cap = st->cap / 2;
        if (new_cap < 1) new_cap = 1;
        if (new_cap < st->size) new_cap = st->size;

        int* new_data = (int*)realloc(st->data, sizeof(int) * new_cap);
        if (new_data) {
            st->data = new_data;
            st->cap  = new_cap;
        }
    }

    return 1;
}

ArrStack* arr_stack_dtr(ArrStack* st) {

    assert(st);

    free(st->data);
    free(st);

    return NULL;
}