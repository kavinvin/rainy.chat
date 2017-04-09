/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "structure.h"

Node * create(void *data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        // printf("Error creating a new node.\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    pthread_mutex_init(&new_node->lock, NULL);
    return new_node;
}

Node * append(List *list, Node *new_node) {
    Node *first = list->head;
    if (first == NULL) {
        // link to self
        new_node->next = new_node;
        new_node->prev = new_node;
        return new_node;
    }
    Node *last = first->prev;
    // printf("locking\n");
    nodeLock(&first->lock, NULL, &last->lock, list->count);
    // printf("locked\n");
    new_node->next = first;
    // printf("next linked\n");
    new_node->prev = last;
    // printf("prev linked\n");
    first->prev = new_node;
    // printf("first attatched\n");
    last->next = new_node;
    // printf("last attatched\n");
    list->count++;
    // increment count
    nodeUnlock(&first->lock, NULL, &last->lock, list->count);
    // printf("unlocked\n");
    if (first == NULL) first = new_node;
    // printf("head switched\n");
    return new_node;
}

Node * delete(List *list, Node *this) {
    if (this == list->head) list->head = this->next;
    if (this == NULL) {
        return NULL;
    }
    Node *prev = this->prev;
    Node *next = this->next;
    if (prev == NULL || next == NULL) {
        return NULL;
    }
    // printf("locking\n");
    nodeLock(&prev->lock, &this->lock, &next->lock, list->count);
    // printf("locked\n");
    prev->next = next;
    next->prev = prev;
    // printf("unlinked\n");
    this->next = NULL;
    this->prev = NULL;
    // printf("detached\n");
    list->count--;
    // decrement count
    if (list->count == 0) list->head = NULL;
    // assign null pointer to head
    nodeUnlock(&prev->lock, &this->lock, &next->lock, list->count);
    // printf("unlocked\n");
    pthread_mutex_destroy(&this->lock);
    // destroy mutex
    free(this);
    // printf("freed\n");
    return next;
}

void nodeLock(pthread_mutex_t *prev, pthread_mutex_t *this, pthread_mutex_t *next, int count) {
    if (count > 1 && prev != NULL) pthread_mutex_lock(prev);
    if (this != NULL) pthread_mutex_lock(this);
    if (count > 2 && next != NULL) pthread_mutex_lock(next);
}

void nodeUnlock(pthread_mutex_t *prev, pthread_mutex_t *this, pthread_mutex_t *next, int count) {
    if (count > 1 && prev != NULL) pthread_mutex_unlock(prev);
    if (this != NULL) pthread_mutex_unlock(this);
    if (count > 2 && next != NULL) pthread_mutex_unlock(next);
}

void map(Node *this, callback function, void *argument) {
    Node *cursor = this->next;
    User *user = (User*)cursor->data;
    while (cursor != this && cursor != NULL) {
        function(cursor, argument);
        cursor = cursor->next;
    }
}
