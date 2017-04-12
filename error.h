/**
  @file  server.h
  @brief Instant messaging API
*/

#ifndef ERROR_H_
#define ERROR_H_

#include "includes.h"

#define SUCCESS 0
#define CLIENT_DISCONNECT 1
#define INVALID_HEADER 2

void sig_handler(int signal);

#endif
