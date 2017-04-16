/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "structure.h"

/**
 * Function: create
 * ----------------------------
 *   create new node linked to null
 *   return new node
 */
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

/**
 * Function: append
 * ----------------------------
 *   append new node to the given list
 *   return new node
 */
Node * append(List *list, Node *this) {
    printlog("Adding user...\n");
    pthread_mutex_lock(&list->lock);
    if (list->len == 0) {
        // link to self
        this->next = this;
        this->prev = this;
        this->attached = 1;
        list->len++;
        printlog("User online: %d\n", list->len);
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
    printlog("User online: %d\n", list->len);
    return this;
}

/**
 * Function: delete
 * ----------------------------
 *   remove selected node from the given list
 *   return next available node
 */
Node * delete(List *list, Node *this) {
    printlog("Removing user...\n");
    if (!this->attached) {
        // node to be delete is in alredy in detached state
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
    // decrement length
    list->len--;
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
    printlog("User online: %d\n", list->len);
    return this;
}

/**
 * Function: map
 * ----------------------------
 *   apply a function to all items except the given node
 *   return 0 if success, -1 if failed
 */
int map(Node *this, callback function, void *argument, int flag) {
    Node *cursor = this;
    if (flag == ALL) {
        // to all node in the list
        do {
            if (function(cursor, argument) < 0) {
                return -1;
            }
            cursor = cursor->next;
        } while (cursor != this && cursor != NULL);
    } else if (flag == SELF) {
        // only to self
        if (function(cursor, argument) < 0) {
            return -1;
        }
    } else if (flag == OTHER) {
        // to other node in the list
        cursor = cursor->next;
        while (cursor != this && cursor != NULL) {
            if (function(cursor, argument) < 0) {
                return -1;
            }
            cursor = cursor->next;
        }
    }
    return 0;
}
