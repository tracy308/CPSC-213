#pragma once

// TODO 7: Add declaration of generic sort procedure by uncommenting next line and replacing ... with appropriate parameter(s)
void sort (void* list, int length, int (*cmp) (void*, int, int), void (*swap) (void*, int, int));
void test_sort();
