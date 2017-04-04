/**
  @file  websocket.h
  @brief A header file include websocket function
*/

#include "websocket.h"

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

int open_handshake(int sockfd) {
    char cli_handshake[BUFFERSIZE], serv_handshake[200], *hkey, *hvalue, *part, *sec_ws_key, *sec_ws_accept;
    int state;

    printf("%d\n", sockfd);

    // receive message from the client to buffer
    memset(&cli_handshake, 0, sizeof(cli_handshake));
    if (recv(sockfd, cli_handshake, BUFFERSIZE, 0) < 0) {
        printf("%s\n", "ERROR on receiving handshake message");
        pthread_exit(NULL);
    }

    part = strtok(cli_handshake, "\r");
    while (1) {
        hkey = strtok(NULL, "\r\n ");
        if (hkey == NULL) {
            break;
        }
        hvalue = strtok(NULL, "\r");
        if (strcmp(hkey, "Sec-WebSocket-Key:") == 0) {
            sec_ws_key = hvalue;
        }
        printf("%s: %s\n", hkey, hvalue);
    }

    // sha1, encode64
    sec_ws_accept = slice(get_handshake_key(sec_ws_key), 0, 28);
    printf("==%s==\n", sec_ws_accept);

    // compose server handshake message
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(sockfd, serv_handshake, strlen(serv_handshake), 0);
    printf("%s\n", serv_handshake);

    return 1;

}

void ws_send(int sockfd, http_frame *frame) {
    char buffer[BUFFERSIZE];

    // write http frame to buffer
    buffer[0] = frame->opcode;
    buffer[1] = frame->size;
    memcpy(buffer+2, frame->message, frame->size);

    // send buffer to client
    checkError(send(sockfd, (void *)&buffer, 2+frame->size, 0),
               "Error on sending message",
               "Message sent");
}

void ws_recv(int sockfd, http_frame *frame) {
    int length, hasmask, skip;
    char buffer[BUFFERSIZE], mask[4];
    checkError(recv(sockfd, buffer, BUFFERSIZE, 0),
               "Error on recieving message",
               "Message received");

    hasmask = buffer[1] & 0x80 ? 1 : 0;
    length = buffer[1] & 0x7f;
    if (length < 126) {
        // get mask
        skip = 6; // 2 + 0 + 4
        frame->size = length;
        memcpy(frame->mask, buffer + 2, sizeof(frame->mask));
    } else if (length == 126) {
        printf("%s\n", "size = 126");
        // 2 byte length
        uint16_t len16;
        memcpy(&len16, buffer + 2, sizeof(uint16_t));
        // get mask
        skip = 8; // 2 + 2 + 4
        frame->size = ntohs(len16);
        memcpy(frame->mask, buffer + 4, sizeof(frame->mask));
    } else if (length == 127) {
        printf("%s\n", "size = 127");
        // 8 byte length
        uint64_t len64;
        memcpy(&len64, buffer + 2, sizeof(uint64_t));
        // get mask
        skip = 14; // 2 + 8 + 4
        frame->size = ntohs(len64);
        memcpy(frame->mask, buffer + 10, sizeof(frame->mask));
    }
    printf("%llu\n", frame->size);
    frame->message = malloc(frame->size);
    memset(frame->message, '\0', frame->size);
    memcpy(frame->message, buffer + skip, frame->size);

    // remove mask from data
    for (uint64_t i=0; i<frame->size; i++){
        frame->message[i] = frame->message[i] ^ frame->mask[i % 4];
    }

}

void printname(Node *cursor, void *none) {
    User *user = (User*)(cursor->data);
    printf("%s\n", user->name);
}

void broadcast(Node *cursor, void *frame_void) {
    User *user = (User*)(cursor->data);
    http_frame *frame = (http_frame*)frame_void;

    ws_send(user->socket, frame);
}
