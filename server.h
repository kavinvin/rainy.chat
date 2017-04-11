/**
  @file  server.h
  @brief Instant messaging API
*/

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"

void parseAddr(int argc, char *argv[], char **host, char **port);
int startServer(char *host, char *port);
List *newList(void);
int initMutex(int count, ...);
void forkService(int sockfd, List *all_users);
User *acceptUser(int sockfd);
void *initRecvSession(void *user_param);
void *initServerSession(void *sockfd_param);
int parseMessage(List *all_users, Node *this, char *message);
void clientRequest(List *all_users, Node *this, char *command);
void serverCommand(int *sockfd, char *command);

typedef struct {
    int sockfd;
    List *list;
} pthread_args_t;

pthread_mutex_t mutex_accept;

#endif
