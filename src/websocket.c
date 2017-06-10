/**
 * File: websocket.c
 * ----------------------------
 *   an executable file of websocket function which serve interface
 *   between browser and raw socket
 */

#include "websocket.h"

/**
 * Function: getHandshakeKey
 * ----------------------------
 *   compute |Sec-WebSocket-Accept| header field
 *   return base64-encoded sha1 of the given string
 */
char * getHandshakeKey(char *str) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    char magic[80], *encoded;
    size_t input_length = 20, output_length;

    strcpy(magic, str);
    strcat(magic, GUID);
    SHA1((unsigned char*)magic, strlen(magic), hash);
    encoded = base64_encode(hash, input_length, (size_t*)&output_length);

    return encoded;
}

/**
 * Function: openHandshake
 * ----------------------------
 *   receive handshake message from client and return them
 *   the corresponding header field
 *   return 1 if success, -1 if failed
 */
int openHandshake(User *user) {
    char buffer[BUFFERSIZE], serv_handshake[300], *token, *last, *sec_ws_accept;
    int length, state;
    Header *header = newHeader();
    if (header == NULL) {
        return -1;
    }

    printlog("-- Handshaking-- \n");

    // receive message from the client to buffer
    memset(&buffer, 0, BUFFERSIZE);
    if ( (length = recv(user->socket, buffer, BUFFERSIZE, 0)) < 0 ) {
        printlog("Handshaking failed\n");
        return -1;
    }
    if (length == 0) {
        printlog("Didn't receive any message from client\n");
        return -1;
    }

    header->string = calloc(length+1, sizeof(char));
    if (header->string == NULL) {
        printlog("Memory allocation failed: %s\n", strerror(errno));
        return -1;
    }

    printlog("Header from buffer:\n%s", buffer);
    printlog("Copying buffer to header, length: %d\n", length);
    printlog("Actual strlen length: %d\n", strlen(buffer));
    strncpy(header->string, buffer, length);

    printlog("Header from header->string\n%s", header->string);

    // parse http method
    token = strtok_r(header->string, "\r\n", &last);
    if (token == NULL) {
        printlog("Error blank header\n");
        return -1;
    }
    header->get = token;
    if (strncasecmp("GET / HTTP/1.1", header->get, 14) != 0) {
        printlog("Invalid method\n");
        printlog("%s\n", header->get);
        return -1;
    }

    // save header attribute
    while (token != NULL) {
        if (strncasecmp("Upgrade: ", token, 9) == 0) {
            // Upgrade
            header->upgrade = token+9;
        } else if (strncasecmp("Connection: ", token, 12) == 0) {
            // connection
            header->connection = token+12;
        } else if (strncasecmp("Host: ", token, 6) == 0) {
            // host
            header->host = token+6;
        } else if (strncasecmp("Origin: ", token, 8) == 0) {
            // origin
            header->origin = token+8;

            header->origin_len = strlen(header->origin);
            if (strncasecmp("rainy.chat", header->origin + max(header->origin_len - 10, 0), 10) &&
                strncasecmp("rainy.dev", header->origin + max(header->origin_len - 9, 0), 9)) {
                printlog("Invalid origin\n");
                return -1;
            }
        } else if (strncasecmp("Sec-WebSocket-Key: ", token, 19) == 0) {
            // key
            header->key = token+19;
        } else if (strncasecmp("Sec-WebSocket-Version: ", token, 23) == 0) {
            // version
            header->version = strtol(token+23, (char**)NULL, 10);
        } else if (strncasecmp("Sec-WebSocket-Extensions: ", token, 26) == 0) {
            // extensions
            header->extension = token+26;
        } else if (strncasecmp("Sec-WebSocket-Protocol: ", token, 24) == 0) {
            // protocol
            header->protocol = token+24;
        } else if (strncasecmp("User-Agent: ", token, 12) == 0) {
            // protocol
            header->agent = token+12;
        }
        token = strtok_r(NULL, "\r\n", &last);
    }

    if (header->key == NULL) {
        printlog("No websocket key specified\n");
        return -1;
    }

    if (header->origin == NULL) {
        printlog("No origin specified\n");
        return -1;
    }

    // sha1, encode64
    sec_ws_accept = slice(getHandshakeKey(header->key), 28);

    // compose server handshake message
    strcpy(serv_handshake, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ");
    strcat(serv_handshake, sec_ws_accept);
    strcat(serv_handshake, "\r\n\r\n");

    // return handshake from the server
    state = send(user->socket, serv_handshake, strlen(serv_handshake), 0);
    if (state < 0) {
        return -1;
    }

    free(sec_ws_accept);

    user->header = header;
    return 0;

}

/**
 * Function: newHeader
 * ----------------------------
 *   create new header struct
 *   return new header
 */
Header *newHeader() {
    Header *header = malloc(sizeof(Header));
    if (header == NULL) {
        printlog("Memory allocation failed: %s\n", strerror(errno));
        return NULL;
    }
    header->string = NULL;
    header->get = NULL;
    header->upgrade = NULL;
    header->connection = NULL;
    header->host = NULL;
    header->origin = NULL;
    header->key = NULL;
    header->accept = NULL;
    header->version = 0;
    header->protocol = NULL;
    header->extension = NULL;
    header->agent = NULL;
    return header;
}

/**
 * Function: wsSend
 * ----------------------------
 *   wsSend(user, http_frame)
 *   send http_frame to the given user
 *   return 0 if success, -1 if failed
 */
int wsSend(Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    int skip;
    char buffer[BUFFERSIZE];

    memset(buffer, 0, sizeof(buffer));

    // calculate frame szie
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
    printlog("Message sent to #%d: %s (%s)\n", user->socket, user->name, user->ip_address);
    return 0;
}

/**
 * Function: wsRecv
 * ----------------------------
 *   wsRecv(user, http_frame)
 *   Receive message from user
 *   return 0 if success, -1 if failed
 */
int wsRecv(Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    int opcode, length, hasmask, skip;
    char buffer[BUFFERSIZE];
    memset(buffer, '\0', BUFFERSIZE);
    if (recv(user->socket, buffer, BUFFERSIZE, 0) <= 0) {
        printlog("%s\n", "Error on receiving message");
        return CLIENT_DISCONNECT;
    }

    // parse http frame
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

    // restrict message length
    if (frame->size > 1200) {
        printlog("Message too long\n");
        return MESSAGE_TOO_LONG;
    }

    // allocate memory for the message
    frame->message = malloc(frame->size+1); // warning: memory must be freed
    if (frame->message == NULL) {
        printlog("Memory allocation failed: %s\n", strerror(errno));
        return -1;
    }
    memset(frame->message, '\0', frame->size+1);
    memcpy(frame->message, buffer + skip, frame->size);

    // remove mask from data
    for (uint64_t i=0; i<frame->size; i++){
        frame->message[i] = frame->message[i] ^ frame->mask[i % 4];
    }

    return SUCCESS;
}

/**
 * Function: broadcast
 * ----------------------------
 *   send a message to users according to the given flag
 *   return void
 */
void broadcast(List *user_list, Node *this, char *message, int flag) {
    http_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.opcode = 129;
    frame.message = message;
    frame.size = strlen(frame.message);
    if (flag == RECUR) {
        if (map(user_list->head, sendMessage, &frame, flag) < 0) {
            // even if it named user_list but it's room list..
            removeNode(this->superlist, this);
            pthread_exit(NULL);
        }
        return;
    }
    if (map(this, sendMessage, &frame, flag) < 0) {
        removeNode(this->superlist, this);
        pthread_exit(NULL);
        return;
    }
}

/**
 * Function: forkService
 * ----------------------------
 *   create new thread for each new client
 *   return void
 */
int sendMessage(Node *this, void *frame_void) {
    http_frame *frame = (http_frame*)frame_void;
    if (wsSend(this, frame) < 0) {
        return -1;
    }
    return 0;
}

/**
 * Function: sendStatus
 * ----------------------------
 *   Send other users status to all users
 *   return void
 */
void sendStatus(List *user_list, User *added_user, User *removed_user) {
    json_t *json, *username, *username_list;
    User *user;
    Node *cursor;
    char *message;

    if (user_list->len == 0) {
        return;
    }

    username_list = json_array();
    cursor = user_list->head;

    // gather all online usernames
    do {
        user = (User*)cursor->data;
        username = json_string(user->name);
        json_array_append_new(username_list, username);
        cursor = cursor->next;
    } while (cursor != user_list->head);

    // pack all to json
    json = json_pack("{s:s, s:i, s:o}",
                     "type", "online",
                     "count", user_list->len,
                     "users", username_list);

    if (added_user != NULL) {
        json_object_set_new(json, "added", json_string(added_user->name));
    }
    if (removed_user != NULL) {
        json_object_set_new(json, "removed", json_string(removed_user->name));
    }

    // broadcast status
    message = json_dumps(json, JSON_COMPACT);
    printlog("%s\n", message);
    broadcast(user_list, cursor, message, ALL);
    free(json);
    free(username_list);
    free(message);
}

/**
 * Function: removeNode
 * ----------------------------
 *   Remove user node from the given list
 *   return void
 */
void removeNode(List *list, Node *this) {
    printlog("Removing Node..\n");
    pop(list, this);
    sendStatus(list, NULL, this->data);
    removeUser(this->data);
    free(this);
}

/**
 * Function: removeUser
 * ----------------------------
 *   Free user memory
 *   return void
 */
void removeUser(User *user) {
    printlog("Removing User..\n");
    close(user->socket);
    free(user->name);
    if (user->header != NULL) {
        free(user->header->string);
        free(user->header);
    }
    free(user);
}
