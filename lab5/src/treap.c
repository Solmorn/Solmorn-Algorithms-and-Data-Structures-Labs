#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/treap.h"

typedef struct TreapNode {
    int key;
    unsigned int priority;
    struct TreapNode* left;
    struct TreapNode* right;
} TreapNode;

typedef struct TreapPair {
    TreapNode* left;
    TreapNode* right;
} TreapPair;

typedef struct Treap {
    TreapNode* root;
    unsigned int random_state;
} Treap;

static unsigned int next_random(unsigned int* state) {
    assert(state != NULL);

    unsigned int x = *state;
    x ^= x << 13u;
    x ^= x >> 17u;
    x ^= x << 5u;
    *state = x;

    return x;
}

static TreapNode* node_ctor(int key, unsigned int priority) {
    TreapNode* node = (TreapNode*)calloc(1, sizeof(TreapNode));
    assert(node != NULL);

    node->key = key;
    node->priority = priority;

    return node;
}

static void node_dtor(TreapNode* node) {
    if (node == NULL) {
        return;
    }

    node_dtor(node->left);
    node_dtor(node->right);
    free(node);
}

static TreapPair split(TreapNode* root, int key) {
    if (root == NULL) {
        TreapPair result = {NULL, NULL};
        return result;
    }

    if (key < root->key) {
        TreapPair pair = split(root->left, key);
        root->left = pair.right;
        pair.right = root;
        return pair;
    }

    TreapPair pair = split(root->right, key);
    root->right = pair.left;
    pair.left = root;
    return pair;
}

static TreapNode* merge(TreapNode* left, TreapNode* right) {
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    }

    right->left = merge(left, right->left);
    return right;
}

static bool node_contains(const TreapNode* node, int key) {
    while (node != NULL) {
        if (key == node->key) {
            return true;
        }
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return false;
}

static TreapNode* node_erase(TreapNode* node, int key, bool* erased) {
    assert(erased != NULL);

    if (node == NULL) {
        *erased = false;
        return NULL;
    }

    if (key < node->key) {
        node->left = node_erase(node->left, key, erased);
        return node;
    }
    if (key > node->key) {
        node->right = node_erase(node->right, key, erased);
        return node;
    }

    TreapNode* result = merge(node->left, node->right);
    free(node);
    *erased = true;

    return result;
}

static void* treap_ctor(void) {
    Treap* tree = (Treap*)calloc(1, sizeof(Treap));
    assert(tree != NULL);

    tree->random_state = 0x9e3779b9u;

    return tree;
}

static void treap_dtor(void* data) {
    assert(data != NULL);

    Treap* tree = (Treap*)data;
    node_dtor(tree->root);
    free(tree);
}

static bool treap_insert(void* data, int key) {
    assert(data != NULL);

    Treap* tree = (Treap*)data;
    if (node_contains(tree->root, key)) {
        return false;
    }

    TreapNode* node = node_ctor(key, next_random(&tree->random_state));
    TreapPair pair = split(tree->root, key);
    tree->root = merge(merge(pair.left, node), pair.right);

    return true;
}

static bool treap_erase(void* data, int key) {
    assert(data != NULL);

    Treap* tree = (Treap*)data;
    bool erased = false;
    tree->root = node_erase(tree->root, key, &erased);

    return erased;
}

static bool treap_contains(void* data, int key) {
    assert(data != NULL);

    Treap* tree = (Treap*)data;

    return node_contains(tree->root, key);
}

const TreeVTable TREAP_VTABLE = {
    treap_ctor,
    treap_dtor,
    treap_insert,
    treap_erase,
    treap_contains
};
