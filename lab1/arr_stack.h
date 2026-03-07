#ifndef ARR_STACK_H
#define ARR_STACK_H


typedef struct ArrStack {
    int    *data;
    size_t  size;
    size_t  cap;
} ArrStack;


ArrStack* arr_stack_ctr(size_t size);
int arr_stack_push(ArrStack* st, int* buffer);
int arr_stack_top(ArrStack* st, int* buffer);
int arr_stack_pop(ArrStack* st);
ArrStack* arr_stack_dtr(ArrStack* st);

#endif