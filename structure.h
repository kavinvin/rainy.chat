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
    struct node_t *prev;
    pthread_mutex_t lock;
};

struct thread_shared {
    struct user_t *user;
    struct node_t *users;
};

typedef struct user_t User;
typedef struct room_t Room;
typedef struct node_t Node;
typedef struct thread_shared ThreadShared;

typedef void (*callback)(Node *data, void *argument);
Node * create(void *data, Node *next, Node *prev);
Node * insert(Node *head, void *data);
void delete(Node *this);
void map(Node *head, callback function, void *argument);

Node *head;
Node *tail;

#endif
