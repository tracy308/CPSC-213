#include <stdio.h>
#include <string.h>
#include "sort.h"


int compare(void* list, int a, int b) {
    char** chars = (char**) list;
    return strcmp (chars[a], chars[b]);
}

void swap(void* list, int a, int b) {
    char** l = list;
    char* temp = l[a];
    l[a] = l[b];
    l[b] = temp;
}

int intComp(void* l, int a, int b) {
    int* list = (int*) l;
    if (list[a] < list[b] ) {
        return -1;
    } else if (list[a] > list[b]) {
        return 1;
    } else {
        return 0;
    }
}

void swapInt(void* list, int a, int b) {
    int* l = list;
    int temp = l[a];
    l[a] = l[b];
    l[b] = temp;
}

// TODO 7: Change this procedure sort generic arrays (i.e., arrays of any type) 
//         by replacing char** with void* and adding function pointer parameters.
//         Update test_sort to use the new sort to sort 
//             (a) this arrays of strings; AND
//             (b) an array of integers (this will help you with TODO 9)
//         See also TODO 8 in sort.h
void sort(void* list, int length, int (*cmp) (void*, int, int), void (*swap) (void*, int, int)) {
    // void* l = list;
    for (int i = 0; i < length; i++)
        for (int j = i + 1; j < length; j++)
            if (cmp (list, i, j) > 0) 
                swap(list, i, j);
}

void test_sort() {
    char* list[] = {"Cat", "Elephant", "Dog", "Lion", "Zebra", "Ape"};
    sort (list, 6, compare, swap);
    for (int i = 0; i < 6; i++)
        printf("%s\n", list[i]);

    int intlist[] = {6,5,4,9,2,1,0}; 
    sort (intlist, 7, intComp, swapInt);
    for (int i = 0; i < 7; i++)
        printf("%d\n", intlist[i]);
}