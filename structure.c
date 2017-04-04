/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "structure.h"

node * create(void *data, node *next) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;

    return new_node;
}

node * prepend(void *data, node *head) {
    node *new_head = create(data,head);
    return new_head;
}

void map(node *head, callback function, void *argument) {
    node* cursor = head;
    while (cursor != NULL) {
        function(cursor, argument);
        cursor = cursor->next;
    }
}
