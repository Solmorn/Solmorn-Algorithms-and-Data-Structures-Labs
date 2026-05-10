#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/skip_list.h"

#define SKIP_LIST_MAX_LEVEL 32

typedef struct SkipNode {
    int key;
    int level;
    struct SkipNode* forward[];
} SkipNode;

typedef struct SkipList {
    SkipNode* header;
    int level;
    unsigned int random_state;
} SkipList;

static unsigned int next_random(unsigned int* state) {
    assert(state != NULL);

    unsigned int x = *state;
    x ^= x << 13u;
    x ^= x >> 17u;
    x ^= x << 5u;
    *state = x;

    return x;
}

static SkipNode* node_ctor(int key, int level) {
    assert(level > 0);
    assert(level <= SKIP_LIST_MAX_LEVEL);

    SkipNode* node = (SkipNode*)calloc(1, sizeof(SkipNode) + (size_t)level * sizeof(SkipNode*));
    assert(node != NULL);

    node->key = key;
    node->level = level;

    return node;
}

static int random_level(SkipList* list) {
    assert(list != NULL);

    int level = 1;
    while (level < SKIP_LIST_MAX_LEVEL && (next_random(&list->random_state) & 1u) != 0u) {
        level++;
    }

    return level;
}

static void* skip_list_ctor(void) {
    SkipList* list = (SkipList*)calloc(1, sizeof(SkipList));
    assert(list != NULL);

    list->header = node_ctor(0, SKIP_LIST_MAX_LEVEL);
    list->level = 1;
    list->random_state = 0x12345678u;

    return list;
}

static void skip_list_dtor(void* data) {
    assert(data != NULL);

    SkipList* list = (SkipList*)data;
    SkipNode* node = list->header->forward[0];

    while (node != NULL) {
        SkipNode* next = node->forward[0];
        free(node);
        node = next;
    }

    free(list->header);
    free(list);
}

static bool skip_list_insert(void* data, int key) {
    assert(data != NULL);

    SkipList* list = (SkipList*)data;
    SkipNode* update[SKIP_LIST_MAX_LEVEL] = {0};
    SkipNode* current = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != NULL && current->key == key) {
        return false;
    }

    int new_level = random_level(list);
    if (new_level > list->level) {
        for (int i = list->level; i < new_level; i++) {
            update[i] = list->header;
        }
        list->level = new_level;
    }

    SkipNode* new_node = node_ctor(key, new_level);
    for (int i = 0; i < new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    return true;
}

static bool skip_list_erase(void* data, int key) {
    assert(data != NULL);

    SkipList* list = (SkipList*)data;
    SkipNode* update[SKIP_LIST_MAX_LEVEL] = {0};
    SkipNode* current = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current == NULL || current->key != key) {
        return false;
    }

    for (int i = 0; i < list->level; i++) {
        if (update[i]->forward[i] != current) {
            break;
        }
        update[i]->forward[i] = current->forward[i];
    }

    free(current);

    while (list->level > 1 && list->header->forward[list->level - 1] == NULL) {
        list->level--;
    }

    return true;
}

static bool skip_list_contains(void* data, int key) {
    assert(data != NULL);

    SkipList* list = (SkipList*)data;
    SkipNode* current = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    return current != NULL && current->key == key;
}

const TreeVTable SKIP_LIST_VTABLE = {
    skip_list_ctor,
    skip_list_dtor,
    skip_list_insert,
    skip_list_erase,
    skip_list_contains
};
