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
    node *new_head = create(data, head);
    return new_head;
}

node * append(void *data, node *tail) {
    node *new_tail = create(data, NULL);
    tail->next = new_tail;
    return new_tail;
}

node * getBefore(node *current, node *head) {
    node *cursor = head;
    while (cursor != NULL) {
        if (cursor->next == current) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

node * removeFront(node *head) {
    if (head == NULL) {
        return NULL;
    }
    node *front = head;
    head = head->next;
    front->next = NULL;
    // if head is the last node in the list
    if (front == head) {
        head = NULL;
    }
    free(front);
    return head;
}

node * removeBack(node *tail) {
    if (tail == NULL) {
        return NULL;
    }
    node *back = tail;
    tail = getBefore(tail);
    if (tail != NULL) {
        tail->next = NULL;
    }
    free(back);
    return tail;
}

node * pop(node *toRemove, node *head, node *tail) {
    if (toRemove == head) {
        head = removeFront(head);
        return head;
    }
    if (toRemove->next == NULL) {
        tail = removeBack(head);
        return tail;
    }
    node *before = getBefore(toRemove, head);
    if (before =! NULL) {
        before->next = NULL;
    }
    toRemove->next = NULL;
    free(toRemove);
    return head;
}

void map(node *head, callback function, void *argument) {
    node* cursor = head;
    while (cursor != NULL) {
        function(cursor, argument);
        cursor = cursor->next;
    }
}
