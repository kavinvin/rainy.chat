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

FILE *logfile;

int ord(char letter);
char chr(int number);
char * getLine();
char * slice(char *string, int stop);
char * rstrip(char *string, char stripper);
void printBits(size_t const size, void const * const ptr);
void strLower(char *string);
void printlog(char *format, ...);

#endif
