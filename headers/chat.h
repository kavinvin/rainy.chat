/**
  @file  chat.h
  @brief A header file to apply common interface and language between
        the client and a server sides.
*/

#ifndef CHAT_H_
#define CHAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>

#define BUFFER_SIZE 256
#define NUM_THREADS 8

typedef struct {
    char name[64];
    int id;
    int socket[2];
} User;

typedef struct {
    char name[64];
    int id;
} Room;

#endif
