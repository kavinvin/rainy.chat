/**
  @file  structure.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#include "includes.h"

#ifndef STRUCTURE_H_
#define STRUCTURE_H_

struct room_t;

struct user_t {
    char *name;
    int socket;
    pthread_t thread_id;
    struct user_t *next;
    // struct room_t *rooms;
};

struct room_t {
    char name[51];
    struct room_t *next;
    struct user_t *users;
};

struct node_t {
    void *data;
    struct node_t *next;
};

struct thread_shared {
    struct user_t *user;
    struct node_t *users;
};

typedef struct user_t User;
typedef struct room_t Room;
typedef struct node_t node;
typedef struct thread_shared ThreadShared;

typedef void (*callback)(node *data, void *argument);
node * create(void *data, node *next);
node * prepend(void *data, node *head);node * append(void *data, node *tail);
void map(node *head, callback function, void *argument);

node *head;
node *tail;

#endif
