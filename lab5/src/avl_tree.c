#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/avl_tree.h"

typedef struct AvlNode {
    int key;
    int height;
    struct AvlNode* left;
    struct AvlNode* right;
} AvlNode;

typedef struct AvlTree {
    AvlNode* root;
} AvlTree;

static int max_int(int left, int right) {
    return left > right ? left : right;
}

static int height_of(const AvlNode* node) {
    return node == NULL ? 0 : node->height;
}

static AvlNode* node_ctor(int key) {
    AvlNode* node = (AvlNode*)calloc(1, sizeof(AvlNode));
    assert(node != NULL);

    node->key = key;
    node->height = 1;

    return node;
}

static void fix_height(AvlNode* node) {
    assert(node != NULL);

    node->height = max_int(height_of(node->left), height_of(node->right)) + 1;
}

static int balance_factor(const AvlNode* node) {
    assert(node != NULL);

    return height_of(node->right) - height_of(node->left);
}

static AvlNode* rotate_left(AvlNode* node) {
    assert(node != NULL);
    assert(node->right != NULL);

    AvlNode* right = node->right;
    node->right = right->left;
    right->left = node;

    fix_height(node);
    fix_height(right);

    return right;
}

static AvlNode* rotate_right(AvlNode* node) {
    assert(node != NULL);
    assert(node->left != NULL);

    AvlNode* left = node->left;
    node->left = left->right;
    left->right = node;

    fix_height(node);
    fix_height(left);

    return left;
}

static AvlNode* fix_balance(AvlNode* node) {
    assert(node != NULL);

    fix_height(node);

    if (balance_factor(node) == 2) {
        if (balance_factor(node->right) < 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }

    if (balance_factor(node) == -2) {
        if (balance_factor(node->left) > 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }

    return node;
}

static AvlNode* node_insert(AvlNode* node, int key, bool* inserted) {
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

    return fix_balance(node);
}

static AvlNode* find_min(AvlNode* node) {
    assert(node != NULL);

    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

static AvlNode* remove_min(AvlNode* node) {
    assert(node != NULL);

    if (node->left == NULL) {
        return node->right;
    }

    node->left = remove_min(node->left);

    return fix_balance(node);
}

static AvlNode* node_erase(AvlNode* node, int key, bool* erased) {
    assert(erased != NULL);

    if (node == NULL) {
        *erased = false;
        return NULL;
    }

    if (key < node->key) {
        node->left = node_erase(node->left, key, erased);
        return fix_balance(node);
    }
    if (key > node->key) {
        node->right = node_erase(node->right, key, erased);
        return fix_balance(node);
    }

    *erased = true;
    AvlNode* left = node->left;
    AvlNode* right = node->right;
    free(node);

    if (right == NULL) {
        return left;
    }

    AvlNode* min_node = find_min(right);
    min_node->right = remove_min(right);
    min_node->left = left;

    return fix_balance(min_node);
}

static bool node_contains(const AvlNode* node, int key) {
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

static void node_dtor(AvlNode* node) {
    if (node == NULL) {
        return;
    }

    node_dtor(node->left);
    node_dtor(node->right);
    free(node);
}

static void* avl_tree_ctor(void) {
    AvlTree* tree = (AvlTree*)calloc(1, sizeof(AvlTree));
    assert(tree != NULL);

    return tree;
}

static void avl_tree_dtor(void* data) {
    assert(data != NULL);

    AvlTree* tree = (AvlTree*)data;
    node_dtor(tree->root);
    free(tree);
}

static bool avl_tree_insert(void* data, int key) {
    assert(data != NULL);

    AvlTree* tree = (AvlTree*)data;
    bool inserted = false;
    tree->root = node_insert(tree->root, key, &inserted);

    return inserted;
}

static bool avl_tree_erase(void* data, int key) {
    assert(data != NULL);

    AvlTree* tree = (AvlTree*)data;
    bool erased = false;
    tree->root = node_erase(tree->root, key, &erased);

    return erased;
}

static bool avl_tree_contains(void* data, int key) {
    assert(data != NULL);

    AvlTree* tree = (AvlTree*)data;

    return node_contains(tree->root, key);
}

const TreeVTable AVL_TREE_VTABLE = {
    avl_tree_ctor,
    avl_tree_dtor,
    avl_tree_insert,
    avl_tree_erase,
    avl_tree_contains
};
