/**
  @file  helper.h
  @brief header file included helper functions
*/

#ifndef HELPER_H_
#define HELPER_H_

#include "structure.h"

#define SIZE 64

#define max(a,b) a > b ? a : b

#define min(a,b) a < b ? a : b

int ord(char letter);
char chr(int number);
char * getLine();
char * slice(char *string, int start, int stop);
char * strip(char *string, char stripper);
char ** split(char *string, char delimiter);
void printBits(size_t const size, void const * const ptr);
void strLower(char *string);

#endif
