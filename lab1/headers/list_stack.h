#ifndef LIST_STACK_H
#define LIST_STACK_H

#include <stddef.h>

typedef struct Node {
    void*        value;
    struct Node* next;
} Node;

typedef struct ListStack {
    Node*  head;
    size_t elem_size;
} ListStack;

ListStack* list_stack_ctr(size_t elem_size);
int list_stack_push(ListStack* st, const void* buffer);
int list_stack_top(ListStack* st, void* buffer);
int list_stack_pop(ListStack* st);
ListStack* list_stack_dtr(ListStack* st);

#endif
