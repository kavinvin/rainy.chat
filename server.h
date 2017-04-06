/**
  @file  server.h
  @brief Instant messaging API
*/

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"

void initClient(int *sockfd);
void *initRecvSession(void *user_param);
void *initServerSession(void *sockfd_param);
int parseMessage(Node *this, char *message);
void clientRequest(Node *this, char *command);
void serverCommand(int *sockfd, char *command);

#endif
