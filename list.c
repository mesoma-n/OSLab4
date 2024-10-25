#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list_t* list_alloc() {
    list_t* mylist = (list_t *) malloc(sizeof(list_t));
    if (mylist == NULL) return NULL;
    mylist->head = NULL;
    return mylist;
}

void list_free(list_t *l) {
    node_t* current = l->head;
    node_t* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(l);
}

void list_print(list_t *l) {
    node_t* current = l->head;
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

int list_length(list_t *l) {
    int length = 0;
    node_t* current = l->head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

void list_add_to_back(list_t *l, elem value) {
    node_t* new_node = (node_t *) malloc(sizeof(node_t));
    if (new_node == NULL) return;
    new_node->value = value;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
    } else {
        node_t* current = l->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void list_add_to_front(list_t *l, elem value) {
    node_t* new_node = (node_t *) malloc(sizeof(node_t));
    if (new_node == NULL) return;
    new_node->value = value;
    new_node->next = l->head;
    l->head = new_node;
}

void list_add_at_index(list_t *l, elem value, int index) {
    if (index < 0) return;

    if (index == 0) {
        list_add_to_front(l, value);
        return;
    }

    node_t* new_node = (node_t *) malloc(sizeof(node_t));
    if (new_node == NULL) return;
    new_node->value = value;

    node_t* current = l->head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        free(new_node);
        return;
    }

    new_node->next = current->next;
    current->next = new_node;
}

elem list_remove_from_back(list_t *l) {
    if (l->head == NULL) return -1;

    node_t* current = l->head;
    if (current->next == NULL) {
        int value = current->value;
        free(current);
        l->head = NULL;
        return value;
    }

    while (current->next->next != NULL) {
        current = current->next;
    }

    int value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

elem list_remove_from_front(list_t *l) {
    if (l->head == NULL) return -1;

    node_t* temp = l->head;
    int value = temp->value;
    l->head = l->head->next;
    free(temp);
    return value;
}

elem list_remove_at_index(list_t *l, int index) {
    if (index < 0 || l->head == NULL) return -1;

    if (index == 0) return list_remove_from_front(l);

    node_t* current = l->head;
    for (int i = 0; i < index - 1 && current->next != NULL; i++) {
        current = current->next;
    }

    if (current->next == NULL) return -1;

    node_t* temp = current->next;
    int value = temp->value;
    current->next = temp->next;
    free(temp);
    return value;
}

bool list_is_in(list_t *l, elem value) {
    node_t* current = l->head;
    while (current != NULL) {
        if (current->value == value) return true;
        current = current->next;
    }
    return false;
}

elem list_get_elem_at(list_t *l, int index) {
    if (index < 0) return -1;

    node_t* current = l->head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    return (current == NULL) ? -1 : current->value;
}

int list_get_index_of(list_t *l, elem value) {
    int index = 0;
    node_t* current = l->head;
    while (current != NULL) {
        if (current->value == value) return index;
        current = current->next;
        index++;
    }
    return -1;
}
