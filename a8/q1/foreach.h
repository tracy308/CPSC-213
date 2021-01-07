#pragma once

void foreach(
    void* collection, 
    void* (*iterator)        (void* collection), 
    int   (*has_next)        (void* iterator), 
    void* (*get_next)        (void* iterator), 
    void  (*delete_iterator) (void* iterator),
    void  (*callback)        (void* element)
);
