/**
 * File: helper.c
 * ----------------------------
 *   basic helper function
 */

#include "helper.h"

/**
 * Function: ord
 * ----------------------------
 *   Convert an ASCII character to an integer
 *   return an integer
 */
int ord(char letter) {
    return (int)letter - 48;
}

/**
 * Function: chr
 * ----------------------------
 *   convert an integer to an ASCII character
 *   return a character
 */
char chr(int number) {
    return (char)number + 48;
}

/**
 * Function: getLine
 * ----------------------------
 *   get a line from stdin
 *   return string
 */
char * getLine() {
    // get a line of input
    char *string;
    string = (char *)calloc(SIZE, sizeof(char));
    scanf("%[^\n]%*c", string);
    return string;
}

/**
 * Function: slice
 * ----------------------------
 *   slice string from the right
 *   return sliced string
 */
char * slice(char *string, int stop) {
    size_t length = strlen(string);
    for (int i=stop; i<length; i++) {
        string[i] = '\0';
    }
    return string;
}

/**
 * Function: rstrip
 * ----------------------------
 *   strip a given character from a string
 *   return stripped string
 */
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

/**
 * Function: printBits
 * ----------------------------
 *   Print data in bits from the given memory
 *   (Asssume Big Endian)
 *   return void
 */
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

/**
 * Function: strLower
 * ----------------------------
 *   Convert whole string to lowercase
 *   return void
 */
void strLower(char *string) {
    for (; *string; ++string) *string = tolower(*string);
}

/**
 * Function: ntohl64
 * ----------------------------
 *   convert network byte order to host byte order
 *   return converted value
 */
uint64_t ntohl64(uint64_t value) {
    static const int num = 42;

    if (*(char *)&num == 42) {
        const uint32_t high = (uint32_t)(value >> 32);
        const uint32_t low = (uint32_t)(value & 0xFFFFFFFF);

        return (((uint64_t)(htonl(low))) << 32) | htonl(high);
    } else {
        return value;
    }
}

/**
 * Function: printlog
 * ----------------------------
 *   Output the given string format to stdout and log file
 *   return void
 */
void printlog(char *format, ...) {
    pthread_mutex_lock(&mutex_log);
    logfile = fopen("rainy.log", "a");
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
