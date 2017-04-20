/**
 * File: structure.c
 * ----------------------------
 *   an executable file for defining structure
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

    // assign default attribute
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->superlist = NULL;
    new_node->sublist = NULL;
    new_node->users = NULL;
    new_node->attached = 0;

    // init mutex on new node
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
    printlog("Appending to list...\n");

    // lock mutex: locking state
    pthread_mutex_lock(&list->lock);
    if (list->len == 0) {
        // link to self
        this->next = this;
        this->prev = this;
        this->attached = 1;
        list->len++;
        printlog("List length: %d\n", list->len);
        list->head = this;
        pthread_mutex_unlock(&list->lock);
        return this;
    }

printf("%d\n", list->len);
printf("Assigning\n");
    Node *first = list->head;
    Node *last = first->prev;
printf("locking\n");

    // lock mutex: inserting state
    pthread_mutex_lock(&first->lock);
    if (first != last) pthread_mutex_lock(&last->lock);

    pthread_mutex_unlock(&list->lock);

printf("attaching\n");
    // relink neighbor node
    this->next = first;
    this->prev = last;
    first->prev = this;
    last->next = this;
    list->len++;
    this->attached = 1;

    // unlock mutex
    pthread_mutex_unlock(&first->lock);
    if (first != last) pthread_mutex_unlock(&last->lock);

    printlog("List length: %d\n", list->len);

    return this;
}

/**
 * Function: pop
 * ----------------------------
 *   remove selected node from the given list
 *   return removed node (must be freed)
 */
Node * pop(List *list, Node *this) {
    printlog("Removing from list...\n");
    if (!this->attached) {
        // node to be pop is in alredy in detached state
        this->next = NULL;
        this->prev = NULL;
        pthread_mutex_destroy(&this->lock);
        return NULL;
    }

    // lock mutex: locking state
    pthread_mutex_lock(&list->lock);

    if (this == list->head) {
        // switch head before pop
        list->head = this->next;
    }

    Node *prev = this->prev;
    Node *next = this->next;

    // lock mutex: detaching state
    pthread_mutex_lock(&this->lock);
    if (this != next) pthread_mutex_lock(&next->lock);
    if (next != prev) pthread_mutex_lock(&prev->lock);

    pthread_mutex_unlock(&list->lock);

    // repair neighbor node link
    prev->next = next;
    next->prev = prev;
    this->next = NULL;
    this->prev = NULL;

    list->len--;
    if (list->len == 0) {
        // assign null pointer to head
        list->head = NULL;
    }

    // unlock mutex
    pthread_mutex_unlock(&this->lock);
    if (this != next) pthread_mutex_unlock(&next->lock);
    if (next != prev) pthread_mutex_unlock(&prev->lock);

    // destory mutex
    pthread_mutex_destroy(&this->lock);

    printlog("List length: %d\n", list->len);

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

/**
 * Function: find
 * ----------------------------
 *   found sublist by name from the given list
 *   return sublist found
 */
Node *get(Node *this, char *name) {
    Node *cursor = this;
    do {
        if (strcmp(cursor->name, name) == 0) {
            return cursor;
        }
        cursor = cursor->next;
    } while (cursor != this);
    return NULL;
}

/**
 * Function: tree
 * ----------------------------
 *   display node tree from the given node recursively
 */
void tree(List *list, int flag) {
    int i;
    if (list->head == NULL) {
        return;
    }
    Node *cursor = list->head;
    do {
        for (i=0; i<list->level; i++) {
            if (flag >> i & 1) {
                printf("    ");
            } else {
                printf("|   ");
            }
        }
        if (cursor->next == list->head) {
            printf("└─ %s\n", cursor->name);
            flag |= 1 << list->level;
        } else {
            printf("|─ %s\n", cursor->name);
        }
        tree(cursor->sublist, flag);
        cursor = cursor->next;
    } while (cursor != list->head);
}

/**
 * Function: newList
 * ----------------------------
 *   create double-circular linked list structure
 *   return new list pointer
 */
List *newList(void) {
    List *list;
    list = malloc(sizeof(List));
    list->len = 0;
    list->level = 0;
    list->head = NULL;

    return list;
}
