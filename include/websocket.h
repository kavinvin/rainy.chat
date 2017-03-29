#include <openssl/sha.h>
#include "h3.h"
#include "base64.h"

char * get_handshake_key(char *str);
void open_handshake(int *sockfd);

char * get_handshake_key(char *str) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    char *magic, *encoded;
    char guid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    size_t input_length = 20, output_length;

    magic = calloc(300, sizeof(char));
    printf("not yet\n");
    strncpy(magic, str, 300);
    printf("not yet\n");
    strcat(magic, guid);
    SHA1((unsigned char*)magic, strlen(magic), hash);
    for (int i=0; i<SHA_DIGEST_LENGTH; i++) {
        printf("%04x\n", hash[i]);
    }
    encoded = base64_encode(hash, input_length, (size_t*)&output_length);

    printf("unsigned = %s\n", str);
    printf("encoded = %s\n", encoded);
    printf("input_length = %zu\n", input_length);
    printf("output_length = %zu\n", output_length);

    return encoded;
}

void open_handshake(int *sockfd) {
    char cli_handshake[500], *hkey, *hvalue, *part, *serv_handshake, *sec_ws_key, *sec_ws_accept;
    int state;

    // receive message from the client to buffer
    memset(&cli_handshake, 0, sizeof(cli_handshake));
    state = recv(*sockfd, cli_handshake, 500, 0);
    checkError(sockfd, "ERROR on accepting", "Accepted");

    printf("++++\n%s\n++++\n", cli_handshake);

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
    sec_ws_accept = get_handshake_key(sec_ws_key);

    // compose server handshake message
    serv_handshake = calloc(200, sizeof(char));
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(*sockfd, serv_handshake, strlen(serv_handshake), 0);
    printf("%s\n", serv_handshake);

}
