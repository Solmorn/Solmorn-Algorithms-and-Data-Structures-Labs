#ifndef LIST_STACK_H
#define LIST_STACK_H

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct ListStack {
    Node *head;
} ListStack;


struct ListStack* list_stack_ctr();
int list_stack_push(ListStack* st, int* buffer);
int list_stack_top(ListStack* st, int* buffer);
int list_stack_pop(ListStack* st);
ListStack* list_stack_dtr(ListStack* st);



#endif