/**
 * File: structure.h
 * ----------------------------
 *   a header file of structure.c
 */

#include "error.h"
#include "helper.h"

#ifndef STRUCTURE_H_
#define STRUCTURE_H_

#define ALL 0
#define SELF 1
#define OTHER 2

typedef struct {
    char *get;
    char *upgrade;
    char *connection;
    char *host;
    char *origin;
    char *key;
    char *accept;
    long version;
    char *protocol;
    char *extension;
    char *agent;
} Header;

struct room_t;

struct user_t {
    char *name;
    char *ip_address;
    int socket;
    pthread_t thread_id;
    int credit;
    Header header;
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
    int attached;
};

typedef struct {
    struct node_t *head;
    int len;
    pthread_mutex_t lock;
} List;

typedef struct user_t User;
typedef struct room_t Room;
typedef struct node_t Node;
typedef struct thread_shared ThreadShared;

typedef int (*callback)(Node *data, void *argument);
Node * create(void *data);
Node * append(List *list, Node *new_node);
Node * delete(List *list, Node *this);
int map(Node *head, callback function, void *argument, int flag);

pthread_mutex_t mutex_log;

#endif
