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
#define RECUR 3

typedef struct {
    char *string;
    char *get;
    char *upgrade;
    char *connection;
    char *host;
    char *origin;
    int origin_len;
    char *key;
    char *accept;
    long version;
    char *protocol;
    char *extension;
    char *agent;
} Header;

struct list_t;
struct room_t;

struct user_t {
    char *name;
    char *ip_address;
    int socket;
    pthread_t thread_id;
    int credit;
    Header *header;
};

struct node_t {
    char name[64];
    char prefix[256];
    void *data;
    struct node_t *next;
    struct node_t *prev;
    struct list_t *superlist;
    struct list_t *sublist;
    struct list_t *users;
    pthread_mutex_t lock;
    int attached;
};

struct list_t {
    struct node_t *head;
    struct node_t *from;
    int len;
    int level;
    pthread_mutex_t lock;
};

typedef struct user_t User;
typedef struct room_t Room;
typedef struct node_t Node;
typedef struct list_t List;

typedef int (*callback)(Node *data, void *argument);
Node * create(void *data);
Node * append(List *list, Node *new_node);
Node * pop(List *list, Node *this);
int map(Node *head, callback function, void *argument, int flag);
Node *get(Node *this, char *name);
json_t *tree(List *list, json_t *json, int flag);
List *newList(void);

pthread_mutex_t mutex_log;

#endif
