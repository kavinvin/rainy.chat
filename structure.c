/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "structure.h"

Node * create(void *data, Node *next, Node *prev) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Error creating a new node.\n");
        pthread_exit(NULL);
    }
    new_node->data = data;
    new_node->next = next;
    new_node->prev = prev;
    return new_node;
}

Node * insert(Node *head, void *data) {
    if (head == NULL) {
        Node *new_node = create(data, NULL, NULL);
        new_node->next = new_node;
        new_node->prev = new_node;
        return new_node;
    }
    Node *new_node = create(data, head, head->prev);
    head->prev->next = new_node;
    head->prev = new_node;
    return new_node;
}

void delete(Node *this) {
    if (this == NULL) {
        return;
    }
    this->prev->next = this->next;
    this->next->prev = this->prev;
    this->next = NULL;
    this->prev = NULL;
    free(this);
}

void map(Node *this, callback function, void *argument) {
    Node *cursor = this->next;
    User *user = (User*)cursor->data;
    while (cursor != this && cursor != NULL) {
        function(cursor, argument);
        cursor = cursor->next;
    }
}
