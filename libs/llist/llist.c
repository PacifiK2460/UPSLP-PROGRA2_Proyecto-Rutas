#include "llist.h"

int llist_append(LList* list, void* data){
    Node* node = (Node*)malloc(sizeof(Node));
    { // Error checking and data assignment
        if(node == NULL){
            return -1;
        }
        node->data = data;
        node->next = NULL;
    }
    
    if(list->head == NULL){
        list->head = node;
    } else {
        Node* current = list->head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = node;
    }
    list->size++;
    return 0;
}

void* llist_remove(LList* list, int index){
    if(index < 0 || index >= list->size){
        return NULL;
    }
    Node* current = list->head;
    Node* previous = NULL;
    for(int i = 0; i < index; i++){
        previous = current;
        current = current->next;
    }
    if(previous == NULL){
        list->head = current->next;
    } else {
        previous->next = current->next;
    }
    void* data = current->data;
    free(current);
    list->size--;
    return data;
}

int llist_insert(LList* list, int index, void* data){
    if(index < 0 || index > list->size){
        return -1;
    }
    Node* node = (Node*)malloc(sizeof(Node));
    { // Error checking and data assignment
        if(node == NULL){
            return -1;
        }
        node->data = data;
        node->next = NULL;
    }
    if(index == 0){
        node->next = list->head;
        list->head = node;
    } else {
        Node* current = list->head;
        Node* previous = NULL;
        for(int i = 0; i < index; i++){
            previous = current;
            current = current->next;
        }
        previous->next = node;
        node->next = current;
    }
    list->size++;
    return 0;
}

void* llist_get(LList* list, int index){
    if(index < 0 || index >= list->size){
        return NULL;
    }
    Node* current = list->head;
    for(int i = 0; i < index; i++){
        current = current->next;
    }
    return current->data;
}

int llist_set(LList* list, int index, void* data){
    if(index < 0 || index >= list->size){
        return -1;
    }
    Node* current = list->head;
    for(int i = 0; i < index; i++){
        current = current->next;
    }
    current->data = data;
    return 0;
}

int llist_size(LList* list){
    return list->size;
}

LList* llist_clone(LList* source){
    LList* list = (LList*)malloc(sizeof(LList));
    { // Error checking and data assignment
        if(list == NULL){
            return NULL;
        }
        list->head = NULL;
        list->last_accessed_index = 0;
        list->size = 0;
    }
    Node* current = source->head;
    while(current != NULL){
        llist_append(list, current->data);
        current = current->next;
    }
    return list;
}