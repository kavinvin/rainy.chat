/**
  @file  helper.c
  @brief header file included helper functions
*/

#include "helper.h"

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
    // warning: memory leakage
}

char * slice(char *string, int stop) {
    size_t length = strlen(string);
    for (int i=stop; i<length; i++) {
        string[i] = '\0';
    }
    return string;
}

char * rstrip(char *string, char stripper) {
    // strip whitespace from two sides of string
    int length = strlen(string);
    // slice of from the right
    for (int i=length-1; i>=0; i--) {
        if ( !isspace(string[i]) && string[i]!=stripper ) break;
        string[i] = '\0';
    }
    return string;
}

// assumes big endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=0;i<size;i++) {
        for (j=0;j<8;j++) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void strLower(char *string) {
    for (; *string; ++string) *string = tolower(*string);
}

void printLog(char *format, ...) {
    pthread_mutex_lock(&mutex_log);
    logfile = fopen("output/rainy.log", "a");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    va_start(args, format);
    vfprintf(logfile, format, args);
    va_end(args);
    fclose(logfile);
    pthread_mutex_unlock(&mutex_log);
}
