/**
  @file  helper.h
  @brief A header file which included common header functions
*/

#ifndef HELPER_H_
#define HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE 64

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

int ord(char letter);
char chr(int number);
char * getLine();
char * slice(char *string, int start, int stop);
char * strip(char *string, char stripper);
char ** split(char *string, char delimiter);
void strLower(char *string);

int ord(char letter) {
    return (int)letter - 48;
}

char chr(int number) {
    return (char)number + 48;
}

char * getLine() {
    // get a line of input
    char *string;
    string = (char *)calloc(SIZE, sizeof(char));
    scanf("%[^\n]%*c", string);
    return string;
}

char * slice(char *string, int start, int stop) {
    size_t length = strlen(string);
    start = min(length, start);
    stop = min(length, stop);
    for (int i=stop; i<length; i++) {
        string[i] = '\0';
    }
    string+=start;
    return string;
}

char * strip(char *string, char stripper) {
    // strip whitespace from two sides of string
    int length = strlen(string);
    // slice of from the right
    for (int i=length-1; i>=0; i--) {
        if ( !isspace(string[i]) && string[i]!=stripper ) break;
        string[i] = '\0';
    }
    // increment pointer until find non-whitespace character
    while (*string) {
        if ( !isspace(*string) && *string!=stripper ) break;
        string++;
    }
    return string;
}

char ** split(char *string, char delimiter) {
    // split string with given delimiter, return pointers of string
    int count = 0;
    bool breakpoint = true;
    char **parts;
    while (*string) {
        // found non-delimiter character
        if (breakpoint && *string != delimiter) {
            parts[count] = string;
            breakpoint = false;
            count++;
            continue;
        }
        // found delimiter character
        if (*string == delimiter) {
            *string = '\0';
            breakpoint = true;
        }
        string++;
    }
    return parts;
}

void strLower(char *string) {
    for (; *string; ++string) *string = tolower(*string);
}

#endif
