/**
  @file  websocket.h
  @brief A header file include websocket function
*/

#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

#include <openssl/sha.h>
#include "base64.h"
#include "socket.h"

typedef struct {
    uint8_t opcode; // 8 bits
    uint8_t mask[4]; // 0 or 4 bytes
    uint64_t size; // 1, 2 or 8 bytes
    char *message; // x bytes
} http_frame;

char * get_handshake_key(char *str);
int open_handshake(int server_socket);
int wsSend(Node *this, http_frame *frame);
int wsRecv(Node *this, http_frame *frame);
void printname(Node *cursor, void *none);
void broadcast(Node *cursor, void *frame);
void removeNode(List *list, Node *this);
void removeUser(User *user);

#endif

