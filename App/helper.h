#ifndef CHAT_H_
#define CHAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int ord(char letter);
char chr(int number);
char * getLine();
char * strip(char *string);
char ** split(char *string, char delimiter);

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

char * strip(char *string) {
    // strip whitespace from two sides of string
    int length = strlen(string);
    // slice of from the right
    for (int i=length-1; i>=0; i--) {
        if ( !isspace(string[i]) && isascii(string[i]) ) break;
        string[i] = '\0';
    }
    // increment pointer until find non-whitespace character
    while (*string) {
        if ( !isspace(*string) && isascii(*string) ) break;
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
            parts
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

#endif