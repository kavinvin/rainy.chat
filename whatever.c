#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 512
#define NUM_THREADS 8

#include "include/helper.h"
#include "include/chat.h"
#include "include/websocket.h"

int main() {

    http_frame frame;
    uint64_t header;

    printf("\n");
    // printBits(sizeof(dataframe), &dataframe);
    // printf("\n");
    // printf("\n");
    frame.opcode = 129;
    frame.mask = 0;
    strcpy(frame.payload, "Hello!");
    frame.len = strlen(frame.payload);
    memcpy(&header, frame.payload, frame.len);
    header = header << 8 | frame.len;
    header = header << 8 | frame.opcode;
    printBits(sizeof(header), &header);

}
