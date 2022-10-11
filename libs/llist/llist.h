#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>

typedef struct Node{
    void* data;
    struct Node* next;
} Node;

typedef struct LList{
    Node* head;
    int last_accessed_index;
    int size;
} LList;

int llist_append(LList* list, void* data);
void* llist_remove(LList* list, int index);
int llist_insert(LList* list, int index, void* data);
void* llist_get(LList* list, int index);
int llist_set(LList* list, int index, void* data);
int llist_size(LList* list);

#endif