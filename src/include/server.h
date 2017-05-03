/**
 * File: server.h
 * ----------------------------
 *   a header file of server.c
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "websocket.h"
#include "rainybot.h"

enum flag {
    COMMAND_PEER = 1,
    COMMAND_PUBLIC = 2,
    COMMAND_MESSAGE = 4,
    COMMAND_FROMPEER = 8,
    COMMAND_FROMABOVE = 16,
    COMMAND_RAIN = 32,
    COMMAND_ENCRYPT = 64,
    COMMAND_BOT = 128
};

int serveRainyChat(char *host, char *port);
void parseAddr(int argc, char *argv[], char **host, char **port);
int initMutex(int count, ...);
void forkService(int server_socket, List *user_list);
User *acceptUser(int server_socket);
int validateUser(List *user_list, Node *this, http_frame *message);
int getMessage(Node *room, Node *this, http_frame *message);
void *initRecvSession(void *user_param);
void *initServerSession(void *server_socket_param);
int readMessage(List *user_list, Node *this, char *message, char **body);
int clientRequest(List *user_list, Node *this, char *command, char **message);
void serverCommand(int *server_socket, char *command);

typedef struct {
    int server_socket;
    List *list;
    List *global;
} pthread_args_t;

pthread_mutex_t mutex_accept;

#endif
