/**
 * File: helper.h
 * ----------------------------
 *   a header file of helper.c
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "structure.h"

#define SIZE 64
#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

FILE *logfile;

int ord(char letter);
char chr(int number);
char * getLine();
char * slice(char *string, int stop);
char * rstrip(char *string, char stripper);
void printBits(size_t const size, void const * const ptr);
void strLower(char *string);
uint64_t ntohl64(uint64_t value);
void printlog(char *format, ...);
void printtime(void);
int decide(float prob);

#endif
