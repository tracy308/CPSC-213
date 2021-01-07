#include <stdlib.h>
#include "stack.h"


/*** Types ***/

struct element {
    void* value;
    struct element* next;
};

struct st_stack {
    struct element* top;
};


/*** Private procedures ***/

static struct element* new_element(void* value, struct element* next) {
    struct element* el = malloc (sizeof(*el));
    el->value = value;
    el->next = next;
    return el;
}


/*** Public procedures ***/

struct st_stack* st_new() {
    struct st_stack* stack = malloc (sizeof(*stack));
    stack->top = NULL;
    return stack;
}

void st_push(struct st_stack* stack, void* value) {
    stack->top = new_element(value, stack->top);
}

void* st_pop(struct st_stack* stack) {
    if (stack->top) {
        struct element* el = stack->top;
        stack->top = el->next;
        void* value = el->value;
        free(el);
        return value;
    } else
        return NULL;
}

int st_is_empty(struct st_stack* stack) {
    return stack->top == NULL;
}