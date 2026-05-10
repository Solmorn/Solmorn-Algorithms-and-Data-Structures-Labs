#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/b_tree.h"

#define B_TREE_MIN_DEGREE 32
#define B_TREE_MAX_KEYS (2 * B_TREE_MIN_DEGREE - 1)
#define B_TREE_MAX_CHILDREN (2 * B_TREE_MIN_DEGREE)

typedef struct BTreeNode {
    int keys[B_TREE_MAX_KEYS];
    struct BTreeNode* children[B_TREE_MAX_CHILDREN];
    int key_count;
    bool is_leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode* root;
} BTree;

static BTreeNode* node_ctor(bool is_leaf) {
    BTreeNode* node = (BTreeNode*)calloc(1, sizeof(BTreeNode));
    assert(node != NULL);

    node->is_leaf = is_leaf;

    return node;
}

static void node_dtor(BTreeNode* node) {
    if (node == NULL) {
        return;
    }

    if (!node->is_leaf) {
        for (int i = 0; i <= node->key_count; i++) {
            node_dtor(node->children[i]);
        }
    }

    free(node);
}

static int find_key(const BTreeNode* node, int key) {
    assert(node != NULL);

    int idx = 0;
    while (idx < node->key_count && node->keys[idx] < key) {
        idx++;
    }

    return idx;
}

static bool node_contains(const BTreeNode* node, int key) {
    while (node != NULL) {
        int idx = find_key(node, key);
        if (idx < node->key_count && node->keys[idx] == key) {
            return true;
        }
        if (node->is_leaf) {
            return false;
        }
        node = node->children[idx];
    }

    return false;
}

static void split_child(BTreeNode* parent, int idx) {
    assert(parent != NULL);
    assert(parent->children[idx] != NULL);

    BTreeNode* full = parent->children[idx];
    BTreeNode* right = node_ctor(full->is_leaf);
    right->key_count = B_TREE_MIN_DEGREE - 1;

    for (int j = 0; j < B_TREE_MIN_DEGREE - 1; j++) {
        right->keys[j] = full->keys[j + B_TREE_MIN_DEGREE];
    }

    if (!full->is_leaf) {
        for (int j = 0; j < B_TREE_MIN_DEGREE; j++) {
            right->children[j] = full->children[j + B_TREE_MIN_DEGREE];
            full->children[j + B_TREE_MIN_DEGREE] = NULL;
        }
    }

    full->key_count = B_TREE_MIN_DEGREE - 1;

    for (int j = parent->key_count; j >= idx + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[idx + 1] = right;

    for (int j = parent->key_count - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[idx] = full->keys[B_TREE_MIN_DEGREE - 1];
    parent->key_count++;
}

static void insert_nonfull(BTreeNode* node, int key) {
    assert(node != NULL);

    int idx = node->key_count - 1;

    if (node->is_leaf) {
        while (idx >= 0 && node->keys[idx] > key) {
            node->keys[idx + 1] = node->keys[idx];
            idx--;
        }
        node->keys[idx + 1] = key;
        node->key_count++;
        return;
    }

    while (idx >= 0 && node->keys[idx] > key) {
        idx--;
    }
    idx++;

    if (node->children[idx]->key_count == B_TREE_MAX_KEYS) {
        split_child(node, idx);
        if (node->keys[idx] < key) {
            idx++;
        }
    }

    insert_nonfull(node->children[idx], key);
}

static int get_predecessor(BTreeNode* node) {
    assert(node != NULL);

    while (!node->is_leaf) {
        node = node->children[node->key_count];
    }

    return node->keys[node->key_count - 1];
}

static int get_successor(BTreeNode* node) {
    assert(node != NULL);

    while (!node->is_leaf) {
        node = node->children[0];
    }

    return node->keys[0];
}

static void remove_from_leaf(BTreeNode* node, int idx) {
    assert(node != NULL);
    assert(idx >= 0);
    assert(idx < node->key_count);

    for (int i = idx + 1; i < node->key_count; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    node->key_count--;
}

static void merge_children(BTreeNode* node, int idx) {
    assert(node != NULL);
    assert(idx >= 0);
    assert(idx < node->key_count);

    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];
    assert(child != NULL);
    assert(sibling != NULL);

    child->keys[B_TREE_MIN_DEGREE - 1] = node->keys[idx];

    for (int i = 0; i < sibling->key_count; i++) {
        child->keys[i + B_TREE_MIN_DEGREE] = sibling->keys[i];
    }

    if (!child->is_leaf) {
        for (int i = 0; i <= sibling->key_count; i++) {
            child->children[i + B_TREE_MIN_DEGREE] = sibling->children[i];
        }
    }

    for (int i = idx + 1; i < node->key_count; i++) {
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = idx + 2; i <= node->key_count; i++) {
        node->children[i - 1] = node->children[i];
    }

    child->key_count += sibling->key_count + 1;
    node->key_count--;
    free(sibling);
}

static void borrow_from_prev(BTreeNode* node, int idx) {
    assert(node != NULL);
    assert(idx > 0);

    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx - 1];
    assert(child != NULL);
    assert(sibling != NULL);

    for (int i = child->key_count - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->is_leaf) {
        for (int i = child->key_count; i >= 0; i--) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = node->keys[idx - 1];

    if (!child->is_leaf) {
        child->children[0] = sibling->children[sibling->key_count];
    }

    node->keys[idx - 1] = sibling->keys[sibling->key_count - 1];
    child->key_count++;
    sibling->key_count--;
}

static void borrow_from_next(BTreeNode* node, int idx) {
    assert(node != NULL);
    assert(idx >= 0);

    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];
    assert(child != NULL);
    assert(sibling != NULL);

    child->keys[child->key_count] = node->keys[idx];

    if (!child->is_leaf) {
        child->children[child->key_count + 1] = sibling->children[0];
    }

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->key_count; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->is_leaf) {
        for (int i = 1; i <= sibling->key_count; i++) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->key_count++;
    sibling->key_count--;
}

static void fill_child(BTreeNode* node, int idx) {
    assert(node != NULL);
    assert(idx >= 0);

    if (idx != 0 && node->children[idx - 1]->key_count >= B_TREE_MIN_DEGREE) {
        borrow_from_prev(node, idx);
    } else if (idx != node->key_count && node->children[idx + 1]->key_count >= B_TREE_MIN_DEGREE) {
        borrow_from_next(node, idx);
    } else {
        if (idx != node->key_count) {
            merge_children(node, idx);
        } else {
            merge_children(node, idx - 1);
        }
    }
}

static bool remove_from_node(BTreeNode* node, int key);

static bool remove_from_nonleaf(BTreeNode* node, int idx) {
    assert(node != NULL);
    assert(idx >= 0);
    assert(idx < node->key_count);

    int key = node->keys[idx];

    if (node->children[idx]->key_count >= B_TREE_MIN_DEGREE) {
        int predecessor = get_predecessor(node->children[idx]);
        node->keys[idx] = predecessor;
        return remove_from_node(node->children[idx], predecessor);
    }

    if (node->children[idx + 1]->key_count >= B_TREE_MIN_DEGREE) {
        int successor = get_successor(node->children[idx + 1]);
        node->keys[idx] = successor;
        return remove_from_node(node->children[idx + 1], successor);
    }

    merge_children(node, idx);
    return remove_from_node(node->children[idx], key);
}

static bool remove_from_node(BTreeNode* node, int key) {
    assert(node != NULL);

    int idx = find_key(node, key);

    if (idx < node->key_count && node->keys[idx] == key) {
        if (node->is_leaf) {
            remove_from_leaf(node, idx);
            return true;
        }
        return remove_from_nonleaf(node, idx);
    }

    if (node->is_leaf) {
        return false;
    }

    bool last_child = idx == node->key_count;

    if (node->children[idx]->key_count < B_TREE_MIN_DEGREE) {
        fill_child(node, idx);
    }

    if (last_child && idx > node->key_count) {
        return remove_from_node(node->children[idx - 1], key);
    }

    return remove_from_node(node->children[idx], key);
}

static void* b_tree_ctor(void) {
    BTree* tree = (BTree*)calloc(1, sizeof(BTree));
    assert(tree != NULL);

    return tree;
}

static void b_tree_dtor(void* data) {
    assert(data != NULL);

    BTree* tree = (BTree*)data;
    node_dtor(tree->root);
    free(tree);
}

static bool b_tree_insert(void* data, int key) {
    assert(data != NULL);

    BTree* tree = (BTree*)data;

    if (node_contains(tree->root, key)) {
        return false;
    }

    if (tree->root == NULL) {
        tree->root = node_ctor(true);
        tree->root->keys[0] = key;
        tree->root->key_count = 1;
        return true;
    }

    if (tree->root->key_count == B_TREE_MAX_KEYS) {
        BTreeNode* new_root = node_ctor(false);
        new_root->children[0] = tree->root;
        split_child(new_root, 0);
        tree->root = new_root;
    }

    insert_nonfull(tree->root, key);

    return true;
}

static bool b_tree_erase(void* data, int key) {
    assert(data != NULL);

    BTree* tree = (BTree*)data;

    if (tree->root == NULL) {
        return false;
    }

    bool erased = remove_from_node(tree->root, key);
    if (!erased) {
        return false;
    }

    if (tree->root->key_count == 0) {
        BTreeNode* old_root = tree->root;
        if (tree->root->is_leaf) {
            tree->root = NULL;
        } else {
            tree->root = tree->root->children[0];
        }
        free(old_root);
    }

    return true;
}

static bool b_tree_contains(void* data, int key) {
    assert(data != NULL);

    BTree* tree = (BTree*)data;

    return node_contains(tree->root, key);
}

const TreeVTable B_TREE_VTABLE = {
    b_tree_ctor,
    b_tree_dtor,
    b_tree_insert,
    b_tree_erase,
    b_tree_contains
};
