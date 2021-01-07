#pragma once

struct st_stack;

struct st_stack* st_new();
void st_delete(struct st_stack* stack);
void st_push(struct st_stack* stack, void* value);
void* st_pop(struct st_stack* stack);
int st_is_empty(struct st_stack* stack);