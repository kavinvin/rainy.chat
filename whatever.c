#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "include/helper.h"

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

int main() {
    char string[] = "??Hello???";
    printf("%s\n", slice(string, 0, 4));
}
