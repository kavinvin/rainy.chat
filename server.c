/**
  @file  server.c
  @brief Instant messaging API
*/

#include "server.h"
#include <jansson.h>

int main(int argc, char *argv[]) {
    int sockfd;
    char *host = argv[1];
    char *port = argv[2];
    sockfd = initSocket(host, port);
    initClient(&sockfd);
    pthread_exit(NULL);
}

void initClient(int *sockfd) {
    int state;
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    pthread_t *thread_id;
    User *user;
    for (int i=0; i<10; i++) {

        // create user: should test if successfully create a user
        user = malloc(sizeof(User));

        // accept incoming request, create new client socket
        user->socket = accept(*sockfd, (struct sockaddr *) &cli_addr, &clilen);
        thread_id = malloc(sizeof(pthread_t));
        checkError(user->socket,
                   "ERROR on accepting",
                   "Accepted");

        state = pthread_create(thread_id, NULL, initRecvSession, (void *)user);
        if (state){
            printf("ERROR; return code from pthread_create() is %d\n", state);
            exit(-1);
        }

    }

    // close socket
    // printf("sockfd closed\n");
    // close(*sockfd);
}

void *initRecvSession(void *param_user) {
    User *user = (User*)param_user;
    char *message;
    http_frame frame;

    // assign temporary username
    user->name = calloc(20, sizeof(char));
    strcpy(user->name, "anonymous");


    checkError(open_handshake(user->socket),
               "handshaking failed",
               "handshaking succeed");

    // prepend user to the linked list
    head = insert(head, user);
    Node *this = head;

    while (1) {
        // receive message from client
        memset(&frame, 0, sizeof(frame));
        ws_recv(this, &frame);

        message = frame.message;
        parseMessage(user, message);

        // send message to client
        memset(&frame, 0, sizeof(frame));
        frame.opcode = 129;
        frame.message = message;
        frame.size = strlen(frame.message);
        map(this, broadcast, &frame);

    }

    close(user->socket);
    printf("newsockfd no.%d closed\n", user->socket);
    pthread_exit(NULL);
}

int parseMessage(User *user, char *message) {
    if (*message == '/') {
        // command mode
        getCommand(message);
        return 1;
    } else {
        // message mode
        printf("Here is the message from no.%d: %s\n", user->socket, message);
        return 0;
    }
}

void getCommand(char *command) {
    if (strcmp(command, "/exit") == 0) {
        printf("Exited\n");
        pthread_exit(NULL);
    } else {
        printf("Command not found\n");
    }
}
