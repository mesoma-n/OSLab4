#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef int elem;

typedef struct Node {
    elem value;
    struct Node* next;
} node_t;

typedef struct {
    node_t* head;
} list_t;

list_t* list_alloc();
void list_free(list_t* l);
void list_print(list_t* l);
int list_length(list_t* l);
void list_add_to_back(list_t* l, elem value);
void list_add_to_front(list_t* l, elem value);
void list_add_at_index(list_t* l, elem value, int index);
elem list_remove_from_back(list_t* l);
elem list_remove_from_front(list_t* l);
elem list_remove_at_index(list_t* l, int index);
bool list_is_in(list_t* l, elem value);
elem list_get_elem_at(list_t* l, int index);
int list_get_index_of(list_t* l, elem value);

#endif
