#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 512
#define NUM_THREADS 8

#include "include/helper.h"
#include "include/chat.h"
#include "include/websocket.h"

int main() {

    http_frame dataframe;
    uint64_t header;

    printf("\n");
    printBits(sizeof(dataframe), &dataframe);
    printf("\n");
    printf("\n");
    dataframe.opcode = 129;
    dataframe.mask = 0;
    dataframe.payloadlen = 4;
    dataframe.payload[0] = 'H';
    dataframe.payload[1] = 'e';
    dataframe.payload[2] = 'l';
    dataframe.payload[3] = 'o';
    header = dataframe.opcode;
    header = header | dataframe.mask << 8;
    header = header | dataframe.payloadlen << 9;
    header = header | dataframe.payload[0] << 16;
    header = header | dataframe.payload[1] << 17;
    header = header | dataframe.payload[2] << 18;
    header = header | dataframe.payload[3] << 19;
    printBits(sizeof(header), &header);
    // uint16_t value = (highByte << 8) | lowByte ; C = (A<<4) | B ;

}
