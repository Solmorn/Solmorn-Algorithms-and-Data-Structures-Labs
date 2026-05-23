#include <stdlib.h>
#include <string.h>

#include "arr_stack.h"

#define ARR_STACK_START_CAP 1

ArrStack* arr_stack_ctr(size_t elem_size) {
    if (elem_size == 0) return NULL;

    ArrStack* st = (ArrStack*)calloc(1, sizeof(ArrStack));
    if (!st) return NULL;

    st->data = calloc(ARR_STACK_START_CAP, elem_size);
    if (!st->data) {
        free(st);
        return NULL;
    }

    st->size      = 0;
    st->cap       = ARR_STACK_START_CAP;
    st->elem_size = elem_size;

    return st;
}

int arr_stack_push(ArrStack* st, const void* buffer) {
    if (!st || !buffer) return 0;

    if (st->size == st->cap) {
        size_t new_cap = (st->cap == 0) ? 1 : st->cap * 2;
        void* new_data = realloc(st->data, st->elem_size * new_cap);

        if (!new_data) return 0;

        st->data = new_data;
        st->cap  = new_cap;
    }

    memcpy((char*)st->data + st->size * st->elem_size, buffer, st->elem_size);
    st->size++;

    return 1;
}

int arr_stack_top(ArrStack* st, void* buffer) {
    if (!st || !buffer || st->size == 0) return 0;

    memcpy(buffer, (char*)st->data + (st->size - 1) * st->elem_size, st->elem_size);

    return 1;
}

int arr_stack_pop(ArrStack* st) {
    if (!st || st->size == 0) return 0;

    st->size--;

    if (st->cap > 1 && st->size * 4 < st->cap) {
        size_t new_cap = st->cap / 2;
        if (new_cap < 1) new_cap = 1;
        if (new_cap < st->size) new_cap = st->size;

        void* new_data = realloc(st->data, st->elem_size * new_cap);
        if (new_data) {
            st->data = new_data;
            st->cap  = new_cap;
        }
    }

    return 1;
}

ArrStack* arr_stack_dtr(ArrStack* st) {
    if (!st) return NULL;

    free(st->data);
    free(st);

    return NULL;
}
