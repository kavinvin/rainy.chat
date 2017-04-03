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
int open_handshake(int sockfd);
void ws_send(int sockfd, http_frame *frame);
void ws_recv(int sockfd, http_frame *frame);

#endif

