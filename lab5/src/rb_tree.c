#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/rb_tree.h"

typedef enum RbColor {
    RB_RED = 0,
    RB_BLACK = 1
} RbColor;

typedef struct RbNode {
    int key;
    RbColor color;
    struct RbNode* left;
    struct RbNode* right;
    struct RbNode* parent;
} RbNode;

typedef struct RbTree {
    RbNode* root;
    RbNode* nil;
} RbTree;

static RbNode* node_ctor(RbTree* tree, int key) {
    assert(tree != NULL);
    assert(tree->nil != NULL);

    RbNode* node = (RbNode*)calloc(1, sizeof(RbNode));
    assert(node != NULL);

    node->key = key;
    node->color = RB_RED;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;

    return node;
}

static void node_dtor(RbTree* tree, RbNode* node) {
    assert(tree != NULL);

    if (node == tree->nil) {
        return;
    }

    node_dtor(tree, node->left);
    node_dtor(tree, node->right);
    free(node);
}

static RbNode* search_node(RbTree* tree, int key) {
    assert(tree != NULL);

    RbNode* node = tree->root;

    while (node != tree->nil) {
        if (key == node->key) {
            return node;
        }
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return tree->nil;
}

static RbNode* minimum_node(RbTree* tree, RbNode* node) {
    assert(tree != NULL);
    assert(node != NULL);
    assert(node != tree->nil);

    while (node->left != tree->nil) {
        node = node->left;
    }

    return node;
}

static void left_rotate(RbTree* tree, RbNode* node) {
    assert(tree != NULL);
    assert(node != NULL);
    assert(node->right != tree->nil);

    RbNode* right = node->right;
    node->right = right->left;
    if (right->left != tree->nil) {
        right->left->parent = node;
    }

    right->parent = node->parent;
    if (node->parent == tree->nil) {
        tree->root = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }

    right->left = node;
    node->parent = right;
}

static void right_rotate(RbTree* tree, RbNode* node) {
    assert(tree != NULL);
    assert(node != NULL);
    assert(node->left != tree->nil);

    RbNode* left = node->left;
    node->left = left->right;
    if (left->right != tree->nil) {
        left->right->parent = node;
    }

    left->parent = node->parent;
    if (node->parent == tree->nil) {
        tree->root = left;
    } else if (node == node->parent->right) {
        node->parent->right = left;
    } else {
        node->parent->left = left;
    }

    left->right = node;
    node->parent = left;
}

static void insert_fixup(RbTree* tree, RbNode* node) {
    assert(tree != NULL);
    assert(node != NULL);

    while (node->parent->color == RB_RED) {
        if (node->parent == node->parent->parent->left) {
            RbNode* uncle = node->parent->parent->right;
            if (uncle->color == RB_RED) {
                node->parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                node->parent->parent->color = RB_RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                node->parent->color = RB_BLACK;
                node->parent->parent->color = RB_RED;
                right_rotate(tree, node->parent->parent);
            }
        } else {
            RbNode* uncle = node->parent->parent->left;
            if (uncle->color == RB_RED) {
                node->parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                node->parent->parent->color = RB_RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(tree, node);
                }
                node->parent->color = RB_BLACK;
                node->parent->parent->color = RB_RED;
                left_rotate(tree, node->parent->parent);
            }
        }
    }

    tree->root->color = RB_BLACK;
}

static void transplant(RbTree* tree, RbNode* old_node, RbNode* new_node) {
    assert(tree != NULL);
    assert(old_node != NULL);
    assert(new_node != NULL);

    if (old_node->parent == tree->nil) {
        tree->root = new_node;
    } else if (old_node == old_node->parent->left) {
        old_node->parent->left = new_node;
    } else {
        old_node->parent->right = new_node;
    }

    new_node->parent = old_node->parent;
}

static void delete_fixup(RbTree* tree, RbNode* node) {
    assert(tree != NULL);
    assert(node != NULL);

    while (node != tree->root && node->color == RB_BLACK) {
        if (node == node->parent->left) {
            RbNode* sibling = node->parent->right;
            if (sibling->color == RB_RED) {
                sibling->color = RB_BLACK;
                node->parent->color = RB_RED;
                left_rotate(tree, node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == RB_BLACK && sibling->right->color == RB_BLACK) {
                sibling->color = RB_RED;
                node = node->parent;
            } else {
                if (sibling->right->color == RB_BLACK) {
                    sibling->left->color = RB_BLACK;
                    sibling->color = RB_RED;
                    right_rotate(tree, sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = RB_BLACK;
                sibling->right->color = RB_BLACK;
                left_rotate(tree, node->parent);
                node = tree->root;
            }
        } else {
            RbNode* sibling = node->parent->left;
            if (sibling->color == RB_RED) {
                sibling->color = RB_BLACK;
                node->parent->color = RB_RED;
                right_rotate(tree, node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == RB_BLACK && sibling->left->color == RB_BLACK) {
                sibling->color = RB_RED;
                node = node->parent;
            } else {
                if (sibling->left->color == RB_BLACK) {
                    sibling->right->color = RB_BLACK;
                    sibling->color = RB_RED;
                    left_rotate(tree, sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = RB_BLACK;
                sibling->left->color = RB_BLACK;
                right_rotate(tree, node->parent);
                node = tree->root;
            }
        }
    }

    node->color = RB_BLACK;
}

static void* rb_tree_ctor(void) {
    RbTree* tree = (RbTree*)calloc(1, sizeof(RbTree));
    assert(tree != NULL);

    tree->nil = (RbNode*)calloc(1, sizeof(RbNode));
    assert(tree->nil != NULL);

    tree->nil->color = RB_BLACK;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->parent = tree->nil;
    tree->root = tree->nil;

    return tree;
}

static void rb_tree_dtor(void* data) {
    assert(data != NULL);

    RbTree* tree = (RbTree*)data;
    node_dtor(tree, tree->root);
    free(tree->nil);
    free(tree);
}

static bool rb_tree_insert(void* data, int key) {
    assert(data != NULL);

    RbTree* tree = (RbTree*)data;
    RbNode* parent = tree->nil;
    RbNode* node = tree->root;

    while (node != tree->nil) {
        parent = node;
        if (key == node->key) {
            return false;
        }
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    RbNode* new_node = node_ctor(tree, key);
    new_node->parent = parent;

    if (parent == tree->nil) {
        tree->root = new_node;
    } else if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    insert_fixup(tree, new_node);

    return true;
}

static bool rb_tree_erase(void* data, int key) {
    assert(data != NULL);

    RbTree* tree = (RbTree*)data;
    RbNode* node = search_node(tree, key);
    if (node == tree->nil) {
        return false;
    }

    RbNode* removed = node;
    RbColor removed_original_color = removed->color;
    RbNode* child = tree->nil;

    if (node->left == tree->nil) {
        child = node->right;
        transplant(tree, node, node->right);
    } else if (node->right == tree->nil) {
        child = node->left;
        transplant(tree, node, node->left);
    } else {
        removed = minimum_node(tree, node->right);
        removed_original_color = removed->color;
        child = removed->right;

        if (removed->parent == node) {
            child->parent = removed;
        } else {
            transplant(tree, removed, removed->right);
            removed->right = node->right;
            removed->right->parent = removed;
        }

        transplant(tree, node, removed);
        removed->left = node->left;
        removed->left->parent = removed;
        removed->color = node->color;
    }

    free(node);

    if (removed_original_color == RB_BLACK) {
        delete_fixup(tree, child);
    }

    return true;
}

static bool rb_tree_contains(void* data, int key) {
    assert(data != NULL);

    RbTree* tree = (RbTree*)data;

    return search_node(tree, key) != tree->nil;
}

const TreeVTable RB_TREE_VTABLE = {
    rb_tree_ctor,
    rb_tree_dtor,
    rb_tree_insert,
    rb_tree_erase,
    rb_tree_contains
};
