#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 512
#define NUM_THREADS 8

#include "include/helper.h"
#include "include/chat.h"
#include "include/websocket.h"

int main() {

    http_frame cli_frame;
    uint64_t header;

    printf("\n");
    // printBits(sizeof(dataframe), &dataframe);
    // printf("\n");
    // printf("\n");
    cli_frame.opcode = 129;
    cli_frame.mask = 0;
    strcpy(cli_frame.payload, "Hello!");
    cli_frame.payloadlen = strlen(cli_frame.payload);
    memcpy(&header, cli_frame.payload, cli_frame.payloadlen);
    header = header << 8 | cli_frame.payloadlen;
    header = header << 8 | cli_frame.opcode;
    printBits(sizeof(header), &header);

}
