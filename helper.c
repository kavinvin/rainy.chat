/**
  @file  helper.h
  @brief A header file which included common header functions
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

//assumes big endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=0;i<size;i++)
    {
        for (j=0;j<8;j++)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void strLower(char *string) {
    for (; *string; ++string) *string = tolower(*string);
}