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

typedef struct {
    void *data;
    struct Node *next;
} Node;

typedef struct user_t User;
typedef struct room_t Room;

#endif
