/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "structure.h"

Node * create(void *data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Error creating a new node.\n");
        return NULL;
    }
    new_node->data = data;
    pthread_mutex_init(&new_node->lock, NULL);
    return new_node;
}

Node * insert(Node *head, Node *new_node) {
    if (head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        return new_node;
    }
    if (node_count > 1) pthread_mutex_lock(&head->prev->lock);
    pthread_mutex_lock(&head->lock);
    new_node->next = head;
    new_node->prev = head->prev;
    head->prev->next = new_node;
    head->prev = new_node;
    if (node_count > 1) pthread_mutex_unlock(&head->prev->lock);
    pthread_mutex_unlock(&head->lock);
    node_count++;
    return new_node;
}

void delete(Node *this) {
    if (this == NULL) {
        return;
    }
    printf("%s\n", "locking");
    if (node_count > 1) pthread_mutex_lock(&this->prev->lock);
    pthread_mutex_lock(&this->lock);
    if (node_count > 2) pthread_mutex_lock(&this->next->lock);
    printf("%s\n", "locked");
    this->prev->next = this->next;
    this->next->prev = this->prev;
    if (node_count > 1) pthread_mutex_unlock(&this->prev->lock);
    if (node_count > 2) pthread_mutex_unlock(&this->next->lock);
    this->next = NULL;
    this->prev = NULL;
    printf("%s\n", "unlink");
    pthread_mutex_unlock(&this->lock);
    pthread_mutex_destroy(&this->lock);
    printf("%s\n", "unlocked");
    free(this);
    printf("%s\n", "freed");
    node_count--;
}

void map(Node *this, callback function, void *argument) {
    Node *cursor = this->next;
    User *user = (User*)cursor->data;
    while (cursor != this && cursor != NULL) {
        function(cursor, argument);
        cursor = cursor->next;
    }
}
