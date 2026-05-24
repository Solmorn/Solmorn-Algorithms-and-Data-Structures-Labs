#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/splay_tree.h"

typedef struct SplayNode {
    int key;
    struct SplayNode* left;
    struct SplayNode* right;
    struct SplayNode* parent;
} SplayNode;

typedef struct SplayTree {
    SplayNode* root;
} SplayTree;

static SplayNode* node_ctor(int key) {
    SplayNode* node = (SplayNode*)calloc(1, sizeof(SplayNode));
    assert(node != NULL);

    node->key = key;

    return node;
}

static void node_dtor(SplayNode* node) {
    if (node == NULL) {
        return;
    }

    node_dtor(node->left);
    node_dtor(node->right);
    free(node);
}

static void replace_parent_child(SplayTree* tree, SplayNode* old_node, SplayNode* new_node) {
    assert(tree != NULL);
    assert(old_node != NULL);

    SplayNode* parent = old_node->parent;
    if (parent == NULL) {
        tree->root = new_node;
    } else if (parent->left == old_node) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    if (new_node != NULL) {
        new_node->parent = parent;
    }
}

static void rotate_left(SplayTree* tree, SplayNode* node) {
    assert(tree != NULL);
    assert(node != NULL);
    assert(node->right != NULL);

    SplayNode* right = node->right;
    node->right = right->left;
    if (right->left != NULL) {
        right->left->parent = node;
    }

    replace_parent_child(tree, node, right);
    right->left = node;
    node->parent = right;
}

static void rotate_right(SplayTree* tree, SplayNode* node) {
    assert(tree != NULL);
    assert(node != NULL);
    assert(node->left != NULL);

    SplayNode* left = node->left;
    node->left = left->right;
    if (left->right != NULL) {
        left->right->parent = node;
    }

    replace_parent_child(tree, node, left);
    left->right = node;
    node->parent = left;
}

static void splay(SplayTree* tree, SplayNode* node) {
    assert(tree != NULL);
    assert(node != NULL);

    while (node->parent != NULL) {
        SplayNode* parent = node->parent;
        SplayNode* grandparent = parent->parent;

        if (grandparent == NULL) {
            if (parent->left == node) {
                rotate_right(tree, parent);
            } else {
                rotate_left(tree, parent);
            }
        } else if (grandparent->left == parent && parent->left == node) {
            rotate_right(tree, grandparent);
            rotate_right(tree, parent);
        } else if (grandparent->right == parent && parent->right == node) {
            rotate_left(tree, grandparent);
            rotate_left(tree, parent);
        } else if (grandparent->left == parent && parent->right == node) {
            rotate_left(tree, parent);
            rotate_right(tree, grandparent);
        } else {
            rotate_right(tree, parent);
            rotate_left(tree, grandparent);
        }
    }
}

static SplayNode* find_node(SplayTree* tree, int key) {
    assert(tree != NULL);

    SplayNode* node = tree->root;
    SplayNode* last = NULL;

    while (node != NULL) {
        last = node;
        if (key == node->key) {
            splay(tree, node);
            return tree->root;
        }
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (last != NULL) {
        splay(tree, last);
    }

    return NULL;
}

static SplayNode* subtree_max(SplayNode* node) {
    assert(node != NULL);

    while (node->right != NULL) {
        node = node->right;
    }

    return node;
}

static void* splay_tree_ctor(void) {
    SplayTree* tree = (SplayTree*)calloc(1, sizeof(SplayTree));
    assert(tree != NULL);

    return tree;
}

static void splay_tree_dtor(void* data) {
    assert(data != NULL);

    SplayTree* tree = (SplayTree*)data;
    node_dtor(tree->root);
    free(tree);
}

static bool splay_tree_insert(void* data, int key) {
    assert(data != NULL);

    SplayTree* tree = (SplayTree*)data;
    SplayNode* parent = NULL;
    SplayNode* node = tree->root;

    while (node != NULL) {
        parent = node;
        if (key == node->key) {
            splay(tree, node);
            return false;
        }
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    SplayNode* new_node = node_ctor(key);
    new_node->parent = parent;

    if (parent == NULL) {
        tree->root = new_node;
    } else if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    splay(tree, new_node);

    return true;
}

static bool splay_tree_erase(void* data, int key) {
    assert(data != NULL);

    SplayTree* tree = (SplayTree*)data;
    SplayNode* node = find_node(tree, key);

    if (node == NULL || node->key != key) {
        return false;
    }

    SplayNode* left = node->left;
    SplayNode* right = node->right;

    if (left != NULL) {
        left->parent = NULL;
    }
    if (right != NULL) {
        right->parent = NULL;
    }

    free(node);

    if (left == NULL) {
        tree->root = right;
        return true;
    }

    tree->root = left;
    SplayNode* max_node = subtree_max(left);
    splay(tree, max_node);
    tree->root->right = right;
    if (right != NULL) {
        right->parent = tree->root;
    }

    return true;
}

static bool splay_tree_contains(void* data, int key) {
    assert(data != NULL);

    SplayTree* tree = (SplayTree*)data;
    SplayNode* node = find_node(tree, key);

    return node != NULL && node->key == key;
}

const TreeVTable SPLAY_TREE_VTABLE = {
    splay_tree_ctor,
    splay_tree_dtor,
    splay_tree_insert,
    splay_tree_erase,
    splay_tree_contains
};
