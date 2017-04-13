/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "structure.h"

Node * create(void *data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printlog("Error creating a new node.\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->attached = 0;
    pthread_mutex_init(&new_node->lock, NULL);
    return new_node;
}

Node * append(List *list, Node *this) {
    pthread_mutex_lock(&list->lock);
    if (list->len == 0) {
        // link to self
        this->next = this;
        this->prev = this;
        this->attached = 1;
        list->len++;
        printlog("user online: %d\n", list->len);
        list->head = this;
        pthread_mutex_unlock(&list->lock);
        return this;
    }
    pthread_mutex_lock(&list->head->lock);
    Node *first = list->head;
    Node *last = first->prev;
    if (list->len > 1) pthread_mutex_lock(&last->lock);
    pthread_mutex_unlock(&list->lock);

    this->next = first;
    this->prev = last;
    first->prev = this;
    last->next = this;
    list->len++;
    // increment len
    this->attached = 1;
    pthread_mutex_unlock(&first->lock);
    if (list->len > 2) pthread_mutex_unlock(&last->lock);
    printlog("user online: %d\n", list->len);
    return this;
}

Node * delete(List *list, Node *this) {
    if (!this->attached) {
        // node detached is in detached state
        pthread_mutex_destroy(&this->lock);
        free(this);
        return NULL;
    }
    pthread_mutex_lock(&list->lock);
    if (this == list->head) {
        // switch head before delete
        list->head = this->next;
    }
    pthread_mutex_lock(&this->lock);
    Node *prev = this->prev;
    Node *next = this->next;
    if (list->len > 1) pthread_mutex_lock(&next->lock);
    if (list->len > 2) pthread_mutex_lock(&prev->lock);
    pthread_mutex_unlock(&list->lock);
    // assigned
    prev->next = next;
    next->prev = prev;
    this->next = NULL;
    this->prev = NULL;
    list->len--;
    // decrement len
    if (list->len == 0) {
        // assign null pointer to head
        list->head = NULL;
    }
    // assign null pointer to head
    pthread_mutex_unlock(&this->lock);
    if (list->len > 0) pthread_mutex_unlock(&next->lock);
    if (list->len > 1) pthread_mutex_unlock(&prev->lock);
    pthread_mutex_destroy(&this->lock);
    // destroy mutex
    free(this);
    printlog("user online: %d\n", list->len);
    return next;
}

int map(Node *this, callback function, void *argument) {
    Node *cursor = this->next;
    User *user = (User*)cursor->data;
    while (cursor != this && cursor != NULL) {
        if (function(cursor, argument) < 0) {
            return -1;
        }
        cursor = cursor->next;
    }
    return 0;
}
