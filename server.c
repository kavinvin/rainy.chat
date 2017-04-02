/**
  @file  server.c
  @brief Instant messaging API
*/

void initClient(int *sockfd);
void *initRecvSession(void *param);
int parseMessage(int *sockfd, char *message);
void getCommand(char *command);

#include "websocket.h"

int main(int argc, char *argv[]) {
    int sockfd;
    sockfd = initSocket(argv[1], argv[2]);
    initClient(&sockfd);
    pthread_exit(NULL);
}

void initClient(int *sockfd) {
    int *newsockfd, state;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    pthread_t tid[4];

    for (int i=0; i<4; i++) {
        // accept incoming request, create new client socket
        clilen = sizeof(cli_addr);
        newsockfd = malloc(sizeof(int));
        *newsockfd = accept(*sockfd, (struct sockaddr *) &cli_addr, &clilen);
        checkError(*newsockfd,
                   "ERROR on accepting",
                   "Accepted");

        state = pthread_create(&tid[i], NULL, initRecvSession, (void *)newsockfd);
        if (state){
            printf("ERROR; return code from pthread_create() is %d\n", state);
            exit(-1);
        }
    }

    // close socket
    // printf("sockfd closed\n");
    // close(*sockfd);
}

void *initRecvSession(void *param) {
    int *newsockfd = (int*)param;
    char *message;
    http_frame frame;

    checkError(open_handshake(newsockfd),
               "handshaking failed",
               "handshaking succeed");

    while (1) {
        // receive message from client
        memset(&frame, 0, sizeof(frame));
        ws_recv(newsockfd, &frame);

        message = frame.message;
        parseMessage(newsockfd, message);

        // send message to client
        memset(&frame, 0, sizeof(frame));
        frame.opcode = 129;
        frame.message = message;
        frame.size = strlen(frame.message);
        ws_send(newsockfd, &frame);
    }

    close(*newsockfd);
    printf("newsockfd no.%d closed\n", *newsockfd);
    pthread_exit(NULL);
}

int parseMessage(int *sockfd, char *message) {
    if (*message == '/') {
        // command mode
        getCommand(message);
        return 1;
    } else {
        // message mode
        printf("Here is the message from no.%d: %s\n", *sockfd, message);
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
