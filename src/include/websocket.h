/**
 * File: websocket.h
 * ----------------------------
 *   a header file of websocket.c
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

char * getHandshakeKey(char *str);
int openHandshake(User *user);
Header *newHeader();
int wsSend(Node *this, http_frame *frame);
int wsRecv(Node *this, http_frame *frame);
void broadcast(List *user_list, Node *this, char *message, int flag);
int sendMessage(Node *cursor, void *message);
void sendStatus(List *user_list, User *added_user, User *removed_user);
void removeNode(List *list, Node *this);
void removeUser(User *user);

#endif

