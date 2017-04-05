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

int open_handshake(int sockfd) {
    char cli_handshake[BUFFERSIZE], serv_handshake[200], *hkey, *hvalue, *part, *sec_ws_key, *sec_ws_accept;
    int state;

    printf("Client socket id: %d\n", sockfd);

    // receive message from the client to buffer
    memset(&cli_handshake, 0, sizeof(cli_handshake));
    showStatus("Handshaking");
    if (recv(sockfd, cli_handshake, BUFFERSIZE, 0) < 0) {
        printf("%s\n", "ERROR on receiving handshake message");
        close(sockfd);
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

    printf("handshake key: %s\n", sec_ws_key);

    // sha1, encode64
    sec_ws_accept = slice(get_handshake_key(sec_ws_key), 0, 28);

    // compose server handshake message
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(sockfd, serv_handshake, strlen(serv_handshake), 0);

    return 1;

}

void ws_send(Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    char buffer[BUFFERSIZE];

    // write http frame to buffer
    buffer[0] = frame->opcode;
    buffer[1] = frame->size;
    memcpy(buffer+2, frame->message, frame->size);

    // send buffer to client
    if (send(user->socket, (void *)&buffer, 2+frame->size, 0) <= 0) {
        printf("%s\n", "Error on sending message");
        removeNode(this);
    }
    printf("Message sent to: %d\n", user->socket);
}

void ws_recv(Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    int length, hasmask, skip;
    char buffer[BUFFERSIZE], mask[4];
    if (recv(user->socket, buffer, BUFFERSIZE, 0) <= 0) {
        printf("%s\n", "Error on recieving message");
        removeNode(this);
    }

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
    ws_send(cursor, frame);
}

void removeNode(Node *this) {
    printf("%s\n", "Removing node..");
    if (this == head) {
        head = head->next;
        if (head == head->next) {
            head = NULL;
        }
    }
    delete(this);
    printf("%s\n", "Node removed");
    removeUser(this->data);
}

void removeUser(User *user) {
    printf("%s\n", "Removing user..");
    close(user->socket);
    free(user->name);
    // pthread_detach(user->thread_id);
    printf("%s\n", "User removed");
    pthread_exit(NULL);
}
