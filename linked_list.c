#include "linked_list.h"
#include <stdlib.h
#include <stdio.h>

struct list_node *new_node(size_t value) { 
    struct list_node *node = malloc(sizeof(struct list_node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (list->head == NULL) {  
        list->head = node;
    } else {
  
        struct list_node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
}

// Remove and return the value from the head of the list.
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        fprintf(stderr, "Attempt to remove from empty list\n");
        exit(EXIT_FAILURE);
    }
    struct list_node *temp = list->head;
    size_t value = temp->value;
    list->head = temp->next;
    free(temp);
    return value;
}

// Remove and return the value from the tail of the list by traversing from the head.
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        fprintf(stderr, "Attempt to remove from empty list\n");
        exit(EXIT_FAILURE);
    }
    // If there is only one node in the list.
    if (list->head->next == NULL) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }
    // Traverse to the node just before the last node.
    struct list_node *current = list->head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    size_t value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

// Free all nodes in the list.
void free_list(struct linked_list list) {
    struct list_node *current = list.head;
    while (current != NULL) {
        struct list_node *next = current->next;
        free(current);
        current = next;
    }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
