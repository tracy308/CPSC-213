#include <stdlib.h>
#include "stack.h"
#include "integer_tree.h"


/*** Types ***/

struct node {
    int value;
    struct node* left;
    struct node* right;
};

struct it_tree {
    struct node* root;
};

struct it_iterator {
    struct it_tree* tree;
    struct st_stack* cursor;
};


/*** Private procedures ***/


/**
 * Recursively delete node and its children
 */

static void delete_node(struct node* node) {
    if (node) {
        delete_node(node->left);
        delete_node(node->right);
        free(node);
    }
}


/**
 * Create and initialize a new node
 */

static struct node* new_node(int value) {
    struct node* node = malloc(sizeof(*node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}


/**
 * Add value to correct, depth-first, ascending-order position in tree rooted at node
 */

static void add_node(struct node* node, int value) {
    if (value < node->value) {
        if (node->left == NULL) 
            node->left = new_node(value);
        else
            add_node(node->left, value);
    } else {
        if (node->right == NULL)
            node->right = new_node(value);
        else
            add_node(node->right, value);
    }
}


/**
 * Push node and all of its left descendants onto stack in order from node to leaf
 */

static void push_left(struct st_stack* stack, struct node* node) {
    while (node) {
        st_push(stack, node);
        node = node->left;
    }
}



/*** Public Integer Tree procedures ***/


/**
 * Create new tree
 */

struct it_tree* it_new() {
    struct it_tree* tree = malloc(sizeof(*tree));
    tree->root = NULL;
    return tree;
}


/**
 * Delete tree
 */

void it_delete(struct it_tree* tree) {
    delete_node(tree->root);
    free(tree);
}


/**
 * Add value to tree in ascending order
 */

void it_add(struct it_tree* tree, int value) {
    if (tree->root == NULL) 
        tree->root = new_node(value);
    else
        add_node(tree->root, value);
}



/*** Public Integer Tree Iterator procedures ***/


/**
 * Return iterator for tree
 */

void* it_iterator(void* treev) {
    struct it_tree* tree = treev;
    struct it_iterator* iterator = malloc(sizeof(*iterator));
    iterator->tree = tree;
    iterator->cursor = st_new();
    push_left(iterator->cursor, tree->root);
    return iterator;
}


/**
 * Return TRUE iff iterator has more values to produce
 */

int it_has_next(void* iteratorv) {
    struct it_iterator* iterator = iteratorv;
    return ! st_is_empty(iterator->cursor);
}


/**
 * Get next element in iteration and advance iterator's cursor
 */

void* it_get_next(void* iteratorv) {
    struct it_iterator* iterator = iteratorv;
    struct node* node = st_pop (iterator->cursor);
    push_left(iterator->cursor, node->right);
    return &node->value;
}


/**
 * Delete iterator
 */

void it_delete_iterator(void* iteratorv) {
    struct it_iterator* iterator = iteratorv;
    free(iterator->cursor);
    free(iterator);
}