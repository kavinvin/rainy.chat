/**
  @file  server.h
  @brief Instant messaging API
*/

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"

int serveRainyChat(char *host, char *port);
void parseAddr(int argc, char *argv[], char **host, char **port);
List *newList(void);
int initMutex(int count, ...);
void forkService(int server_socket, List *all_users);
User *acceptUser(int server_socket);
int validateUser(Node *this, http_frame *message);
char *getMessage(List *all_users, Node *this, http_frame *message);
void *initRecvSession(void *user_param);
void *initServerSession(void *server_socket_param);
int readMessage(List *all_users, Node *this, char *message);
void sendStatus(List *all_users);
void clientRequest(List *all_users, Node *this, char *command);
void serverCommand(int *server_socket, char *command);

typedef struct {
    int server_socket;
    List *list;
} pthread_args_t;

pthread_mutex_t mutex_accept;

#endif
