#ifndef ARR_STACK_H
#define ARR_STACK_H

#include <stddef.h>

typedef struct ArrStack {
    void*  data;
    size_t size;
    size_t cap;
    size_t elem_size;
} ArrStack;

ArrStack* arr_stack_ctr(size_t elem_size);
int arr_stack_push(ArrStack* st, const void* buffer);
int arr_stack_top(ArrStack* st, void* buffer);
int arr_stack_pop(ArrStack* st);
ArrStack* arr_stack_dtr(ArrStack* st);

#endif
