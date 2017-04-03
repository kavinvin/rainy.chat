/**
  @file  server.h
  @brief Instant messaging API
*/

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"

void initClient(int *sockfd);
void *initRecvSession(void *param);
int parseMessage(int sockfd, char *message);
void getCommand(char *command);

#endif
