#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/naive_tree.h"

typedef struct NaiveNode {
    int key;
    struct NaiveNode* left;
    struct NaiveNode* right;
} NaiveNode;

typedef struct NaiveTree {
    NaiveNode* root;
} NaiveTree;

static NaiveNode* node_ctor(int key) {
    NaiveNode* node = (NaiveNode*)calloc(1, sizeof(NaiveNode));
    assert(node != NULL);

    node->key = key;

    return node;
}

static void node_dtor(NaiveNode* node) {
    if (node == NULL) {
        return;
    }

    node_dtor(node->left);
    node_dtor(node->right);
    free(node);
}

static bool node_contains(const NaiveNode* node, int key) {
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

static NaiveNode* node_insert(NaiveNode* node, int key, bool* inserted) {
    assert(inserted != NULL);

    if (node == NULL) {
        *inserted = true;
        return node_ctor(key);
    }

    if (key == node->key) {
        *inserted = false;
        return node;
    }
    if (key < node->key) {
        node->left = node_insert(node->left, key, inserted);
    } else {
        node->right = node_insert(node->right, key, inserted);
    }

    return node;
}

static NaiveNode* remove_max(NaiveNode* node, int* key) {
    assert(node != NULL);
    assert(key != NULL);

    if (node->right == NULL) {
        NaiveNode* left = node->left;
        *key = node->key;
        free(node);
        return left;
    }

    node->right = remove_max(node->right, key);

    return node;
}

static NaiveNode* node_erase(NaiveNode* node, int key, bool* erased) {
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

    *erased = true;
    if (node->left == NULL) {
        NaiveNode* right = node->right;
        free(node);
        return right;
    }
    if (node->right == NULL) {
        NaiveNode* left = node->left;
        free(node);
        return left;
    }

    int new_key = 0;
    node->left = remove_max(node->left, &new_key);
    node->key = new_key;

    return node;
}

static void* naive_tree_ctor(void) {
    NaiveTree* tree = (NaiveTree*)calloc(1, sizeof(NaiveTree));
    assert(tree != NULL);

    return tree;
}

static void naive_tree_dtor(void* data) {
    assert(data != NULL);

    NaiveTree* tree = (NaiveTree*)data;
    node_dtor(tree->root);
    free(tree);
}

static bool naive_tree_insert(void* data, int key) {
    assert(data != NULL);

    NaiveTree* tree = (NaiveTree*)data;
    bool inserted = false;
    tree->root = node_insert(tree->root, key, &inserted);

    return inserted;
}

static bool naive_tree_erase(void* data, int key) {
    assert(data != NULL);

    NaiveTree* tree = (NaiveTree*)data;
    bool erased = false;
    tree->root = node_erase(tree->root, key, &erased);

    return erased;
}

static bool naive_tree_contains(void* data, int key) {
    assert(data != NULL);

    NaiveTree* tree = (NaiveTree*)data;

    return node_contains(tree->root, key);
}

const TreeVTable NAIVE_TREE_VTABLE = {
    naive_tree_ctor,
    naive_tree_dtor,
    naive_tree_insert,
    naive_tree_erase,
    naive_tree_contains
};
