/**
 * File: server.h
 * ----------------------------
 *   a header file of server.c
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"

int serveRainyChat(char *host, char *port);
void parseAddr(int argc, char *argv[], char **host, char **port);
int initMutex(int count, ...);
void forkService(int server_socket, List *user_list);
User *acceptUser(int server_socket);
int validateUser(List *user_list, Node *this, http_frame *message);
char *getMessage(List *user_list, Node *this, http_frame *message);
void *initRecvSession(void *user_param);
void *initServerSession(void *server_socket_param);
int readMessage(List *user_list, Node *this, char *message);
void clientRequest(List *user_list, Node *this, char *command);
void serverCommand(int *server_socket, char *command);

typedef struct {
    int server_socket;
    List *list;
    List *global;
} pthread_args_t;

pthread_mutex_t mutex_accept;

#endif
