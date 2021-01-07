#include <stdlib.h>
#include "integer_list.h"
#include "sort.h"


/*** Types ***/

struct il_list {
    // TODO 4
    int* items;
    int size;
    int length;
};

struct il_iterator {
    // TODO 5
    struct il_list* list;
    int cursor;
};



/*** Private procedures ***/

/**
 * Compare data[a] and data[b], return -1 if data[a] is smaller, 1 if it is larger, and 0 if they are equal
 */

static int compare(void* datav, int a, int b) {
    // TODO 8
    int* data = datav;
    if (data[a] < data[b]) {
        return -1;
    } else if (data[a] > data[b]) {
        return 1;
    } else {
        return 0;
    }
    // return 0; // PLACEHOLDER
}


/**
 * Swap data[a] and data[b]
 */

static void swap(void* datav, int a, int b) {
    // TODO 8
    int* data = datav;
    int temp = data[a];
    data[a] = data[b];
    data[b] = temp;
}



/*** Public procedures for Integer List ***/


/**
 * Create and initialize a new list
 */

struct il_list* il_new(int size) {
    // TODO 4
    struct il_list* newList = malloc(sizeof(struct il_list));
    newList->items = malloc (sizeof(int) * size);
    newList->size = size;
    newList->length = 0;
    return newList;
}


/**
 * Delete list
 */

void il_delete(struct il_list* list) {
    // TODO 4
    free(list->items);
    free(list);
}


/**
 * Add value to end of list
 */

void il_add(struct il_list* list, int value) {
    if (list->length != list->size) {
        list->items[list->length] = value;
        list->length++;
    }
}


/**
 * Sort list in ascending order by value
 */

void il_sort(struct il_list* list) {
    // TODO 8
    sort(list->items, list->length, compare, swap);
}



/*** Public procedures for Integer List Iterator ***/


/**
 * Create and initialize iterator for list
 */

void* il_iterator(void* listv) {
    // TODO 5
    struct il_list* list = listv;
    struct il_iterator* iterator = malloc(sizeof(*iterator));
    iterator->list = list;
    iterator->cursor = 0;
    return iterator; // PLACEHOLDER
}


/**
 * Return TRUE iff iterator has more values to produce
 */

int il_has_next(void* iteratorv) {
    // TODO 5
    struct il_iterator* iterator = iteratorv;
    return iterator->cursor < iterator->list->length; // PLACEHOLDER
}


/**
 * Get pointer to next value in iteration and advance interator's cursor
 */

void* il_get_next(void* iteratorv) {
    // TODO 5
    struct il_iterator* iterator = iteratorv;
    int* num = &iterator->list->items[iterator->cursor];
    iterator->cursor++;
    return num; // PLACEHOLDER
}


/**
 * Delete iterator
 */
void il_delete_iterator(void* iterator) {
    // TODO 5
    struct il_iterator* delete = iterator;
    free(delete);
}