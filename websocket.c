/**
  @file  websocket.h
  @brief A header file include websocket function
*/

#include "websocket.h"

char * get_handshake_key(char *str) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    char magic[80], *encoded;
    size_t input_length = 20, output_length;

    strcpy(magic, str);
    strcat(magic, GUID);
    SHA1((unsigned char*)magic, strlen(magic), hash);
    encoded = base64_encode(hash, input_length, (size_t*)&output_length);

    return encoded;
}

int open_handshake(int server_socket) {
    char cli_handshake[BUFFERSIZE], serv_handshake[300], *hkey, *hvalue, *part, *sec_ws_key, *sec_ws_accept;
    int state;

    printlog("-- Handshaking-- \n");

    // receive message from the client to buffer
    memset(&cli_handshake, 0, sizeof(cli_handshake));
    if (recv(server_socket, cli_handshake, BUFFERSIZE, 0) < 0) {
        printlog("Handshaking failed\n");
        close(server_socket);
        return -1;
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
            printlog("Sec-WebSocket-Key: %s\n", sec_ws_key);
        }
        if (strcmp(hkey, "User-Agent:") == 0) {
            printlog("User-Agent: %s\n", hvalue);
        }
        // printlog("%s %s\n", hkey, hvalue);
    }

    // sha1, encode64
    sec_ws_accept = slice(get_handshake_key(sec_ws_key), 28);

    // compose server handshake message
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(server_socket, serv_handshake, strlen(serv_handshake), 0);

    free(sec_ws_accept);

    return 0;

}

int wsSend(Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    int skip;
    char buffer[BUFFERSIZE];

    memset(buffer, 0, sizeof(buffer));

    if (frame->size <= 125) {
        skip = 2;
        buffer[1] = frame->size;
    } else if (frame->size <= 65535) {
        uint16_t len16;
        skip = 4;
        buffer[1] = 126;
        len16 = htons(frame->size);
        memcpy(buffer+2, &len16, sizeof(uint16_t));
    } else {
        uint64_t len64;
        skip = 10;
        buffer[1] = 127;
        len64 = htonl(frame->size);
        memcpy(buffer+2, &len64, sizeof(uint64_t));
    }

    // write http frame to buffer
    buffer[0] = frame->opcode;
    memcpy(buffer+skip, frame->message, frame->size);

    // send buffer to client
    if (send(user->socket, (void *)&buffer, frame->size + skip, 0) <= 0) {
        printlog("%s\n", "Error on sending message");
        return -1;
    }
    printlog("Message sent to: %d\n", user->socket);
    return 0;
}

int wsRecv(Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    int opcode, length, hasmask, skip;
    char buffer[BUFFERSIZE], mask[4];
    memset(buffer, '\0', BUFFERSIZE);
    if (recv(user->socket, buffer, BUFFERSIZE, 0) <= 0) {
        printlog("%s\n", "Error on recieving message");
        return CLIENT_DISCONNECT;
    }

    opcode = buffer[0] & 0xff;
    hasmask = buffer[1] & 0x80 ? 1 : 0;
    length = buffer[1] & 0x7f;
    if (opcode != 129) {
        // bad opcode
        printlog("Bad opcode\n");
        return INVALID_HEADER;
    }
    if (!hasmask) {
        // remove opcode
        printlog("Message not masked\n");
        return INVALID_HEADER;
    }
    if (length <= 125) {
        // get mask
        skip = 6; // 2 + 0 + 4
        frame->size = length;
        memcpy(frame->mask, buffer + 2, sizeof(frame->mask));
    } else if (length == 126) {
        printlog("%s\n", "size = 126 extended");
        // 2 byte length
        uint16_t len16;
        memcpy(&len16, buffer + 2, sizeof(uint16_t));
        // get mask
        skip = 8; // 2 + 2 + 4
        frame->size = ntohs(len16);
        memcpy(frame->mask, buffer + 4, sizeof(frame->mask));
    } else if (length == 127) {
        printlog("%s\n", "size = 127 extended");
        // 8 byte length
        uint64_t len64;
        memcpy(&len64, buffer + 2, sizeof(uint64_t));
        // get mask
        skip = 14; // 2 + 8 + 4
        frame->size = ntohl64(len64);
        memcpy(frame->mask, buffer + 10, sizeof(frame->mask));
    }

    if (frame->size > 8000) {
        printlog("Message too long\n");
        return MESSAGE_TOO_LONG;
    }

    frame->message = malloc(frame->size+1); // warning: memory leakage
    memset(frame->message, '\0', frame->size+1);
    memcpy(frame->message, buffer + skip, frame->size);

    // printf("expected msg len: %llu\n", frame->size);

    // remove mask from data
    for (uint64_t i=0; i<frame->size; i++){
        frame->message[i] = frame->message[i] ^ frame->mask[i % 4];
    }
    return SUCCESS;
}

void printname(Node *cursor, void *none) {
    User *user = (User*)(cursor->data);
    printlog("%s\n", user->name);
}

int broadcast(Node *this, void *frame_void) {
    User *user = (User*)(this->data);
    http_frame *frame = (http_frame*)frame_void;
    if (wsSend(this, frame) < 0) {
        return -1;
    }
    return 0;
}

void removeNode(List *list, Node *this) {
    removeUser(this->data);
    delete(list, this);
}

void removeUser(User *user) {
    close(user->socket);
    free(user->name);
    free(user);
}
