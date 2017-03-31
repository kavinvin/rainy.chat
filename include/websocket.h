#include <openssl/sha.h>
#include "base64.h"

typedef struct {
    uint8_t opcode; // 8 bits
    uint8_t mask; // 1 bit
    uint64_t payloadlen; // 7 bits, 7+16 bits, or 7+64 bits
    uint8_t maskkey[4]; // 0 or 4 bytes
    uint8_t payload[12]; // x bytes
} http_frame;

char * get_handshake_key(char *str);
void open_handshake(int *sockfd);
void checkError(int *sockfd, char *errormsg, char *successmsg);

char * get_handshake_key(char *str) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    char magic[80], *encoded;
    char guid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    size_t input_length = 20, output_length;

    strcpy(magic, str);
    strcat(magic, guid);
    SHA1((unsigned char*)magic, strlen(magic), hash);
    encoded = base64_encode(hash, input_length, (size_t*)&output_length);

    return encoded;
}

void open_handshake(int *sockfd) {
    char cli_handshake[BUFFER_SIZE], *hkey, *hvalue, *part, *serv_handshake, *sec_ws_key, *sec_ws_accept;
    int state;

    // receive message from the client to buffer
    memset(&cli_handshake, 0, sizeof(cli_handshake));
    state = recv(*sockfd, cli_handshake, BUFFER_SIZE, 0);
    checkError(sockfd, "ERROR on accepting", "Accepted");

    part = strtok(cli_handshake, "\r");
    while (1) {
        hkey = strtok(NULL, "\r\n:");
        if (hkey == NULL) {
            break;
        }
        hvalue = strtok(NULL, " \r");
        if (strcmp(hkey, "Sec-WebSocket-Key") == 0) {
            sec_ws_key = hvalue;
        }
        printf("%s: %s\n", hkey, hvalue);
    }

    // sha1, encode64
    sec_ws_accept = slice(strip(get_handshake_key(sec_ws_key), '?'), 0, 29);
    printf("==%s==\n", sec_ws_accept);

    // compose server handshake message
    serv_handshake = calloc(200, sizeof(serv_handshake));
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(*sockfd, serv_handshake, strlen(serv_handshake), 0);
    printf("%s\n", serv_handshake);

}


void checkError(int *sockfd, char *errormsg, char *successmsg) {
    if (sockfd < 0) {
        perror(errormsg);
        exit(1);
    }
    printf("-- %s --\n", successmsg);

}

