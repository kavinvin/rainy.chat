/**
  @file  includes.h
  @brief A header file to apply common interface and language between
         the client and a server sides.
*/

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#define BUFFERSIZE 1024
#define NUM_THREADS 8

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

typedef struct user_t User;
typedef struct room_t Room;

#endif
