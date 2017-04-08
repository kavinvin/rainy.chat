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

Node * insert(Node *next, Node *new_node) {
    if (node_count == 0) {
        new_node->next = new_node;
        new_node->prev = new_node;
        node_count = 1;
        printf("%d\n", node_count);
        return new_node;
    }
    Node *prev = next->prev;
    printf("locking\n");
    if (node_count > 1) pthread_mutex_lock(&prev->lock);
    pthread_mutex_lock(&next->lock);
    printf("locked\n");
    new_node->next = next;
    new_node->prev = prev;
    printf("linked\n");
    prev->next = new_node;
    next->prev = new_node;
    printf("attatched\n");
    if (node_count > 1) pthread_mutex_unlock(&prev->lock);
    pthread_mutex_unlock(&next->lock);
    printf("unlocked\n");
    return new_node;
}

Node * delete(Node *this) {
    Node *prev = this->prev;
    Node *next = this->next;
    if (this == NULL) {
        return NULL;
    }
    printf("locking\n");
    if (node_count > 1) pthread_mutex_lock(&prev->lock);
    pthread_mutex_lock(&this->lock);
    if (node_count > 2) pthread_mutex_lock(&next->lock);
    printf("locked\n");
    prev->next = next;
    next->prev = prev;
    printf("unlinked\n");
    if (node_count > 1) pthread_mutex_unlock(&prev->lock);
    if (node_count > 2) pthread_mutex_unlock(&next->lock);
    printf("neighbor unlocked\n");
    this->next = NULL;
    this->prev = NULL;
    printf("detached\n");
    pthread_mutex_unlock(&this->lock);
    pthread_mutex_destroy(&this->lock);
    printf("current unlocked\n");
    free(this);
    printf("freed\n");
    return next;
}

void map(Node *this, callback function, void *argument) {
    Node *cursor = this->next;
    User *user = (User*)cursor->data;
    while (cursor != this && cursor != NULL) {
        function(cursor, argument);
        cursor = cursor->next;
    }
}
