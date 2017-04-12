/**
  @file  server.h
  @brief Instant messaging API
*/

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"

int serve(char *host, char *port);
void parseAddr(int argc, char *argv[], char **host, char **port);
List *newList(void);
int initMutex(int count, ...);
void forkService(int server_socket, List *all_users);
User *acceptUser(int server_socket);
void *initRecvSession(void *user_param);
void *initServerSession(void *server_socket_param);
int parseMessage(List *all_users, Node *this, char *message);
void clientRequest(List *all_users, Node *this, char *command);
void serverCommand(int *server_socket, char *command);

typedef struct {
    int server_socket;
    List *list;
} pthread_args_t;

pthread_mutex_t mutex_accept;

#endif
