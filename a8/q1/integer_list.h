#pragma once

struct il_list;
struct il_iterator;

struct il_list* il_new             (int size);                         // create empty list of bounded size
void            il_delete          (struct il_list* list);             // delete list
void            il_add             (struct il_list* list, int value);  // add value to list
void            il_sort            (struct il_list* list);             // sort list in ascending order
void*           il_iterator        (void* list);                       // returns iterator
int             il_has_next        (void* iterator);                   // returns true iff iterator has more elements to produce
void*           il_get_next        (void* iterator);                   // return pointer to next element and advance iterator's cursor
void            il_delete_iterator (void* iterator);                   // delete iterator
