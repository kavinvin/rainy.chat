/**
  @file  socket.h
  @brief A header file include server socket
*/

#ifndef SOCKET_H_
#define SOCKET_H_

#include "websocket.h"
#include "helper.h"

void checkError(int state, char *errormsg, char *successmsg);
int initSocket(char *host, char *portno);

#endif
