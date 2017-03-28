#include <openssl/sha.h>
#include "base64.h"

char * get_handshake_key(unsigned char *str);
void open_handshake(int *sockfd);

char * get_handshake_key(unsigned char *str) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    char *encoded;
    size_t input_length = 19, output_length;

    SHA1(str, sizeof(str) - 1, hash);
    for (int i=0; i<SHA_DIGEST_LENGTH; i++) {
        printf("%02x\n", hash[i]);
    }
    encoded = base64_encode(hash, sizeof(hash), (size_t*)&output_length);

    printf("unsigned = %s\n", str);
    printf("encoded = %s\n", encoded);

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
    sec_ws_accept = get_handshake_key((unsigned char*)sec_ws_key);

    // compose server handshake message
    serv_handshake = calloc(200, sizeof(serv_handshake));
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(*sockfd, serv_handshake, strlen(serv_handshake), 0);
    printf("%s\n", serv_handshake);

}
