/**
  @file  server.c
  @brief Instant messaging API
*/

#define BUFFER_SIZE 256
#define NUM_THREADS 8


void checkError(int *sockfd, char *errormsg, char *successmsg);
int initSocket(char *host, char *portno);
void initConnection(int *sockfd);
void *initRecvSession(void *param);
int processMessage(char *message);
void searchCommand(char *command);

#include "headers/helper.h"
#include "headers/chat.h"
#include "headers/websocket.h"

int main(int argc, char *argv[]) {
    int sockfd;
    sockfd = initSocket(argv[1], argv[2]);
    initConnection(&sockfd);
    pthread_exit(NULL);
}

int initSocket(char *host, char *portno) {
    int sockfd, state;
    struct sockaddr_in server_address;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    checkError(&sockfd, "ERROR opening socket", "Socket created");

    // create server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(portno)); // htons converts from host byte order to network byte order
    server_address.sin_addr.s_addr = inet_addr(host); // inet_addr convert dot notation into network address in Network Byte Order

    // bind the socket to the server address
    state = bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address));
    checkError(&state, "ERROR on binding", "Socket binded");

    // listening to the socket
    state = listen(sockfd, 5);
    checkError(&state, "ERROR on listening", "Listening");

    return sockfd;
}

void initConnection(int *sockfd) {
    int *newsockfd, state;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    pthread_t tid;

    // accept incoming request, create new client socket
    clilen = sizeof(cli_addr);
    newsockfd = malloc(sizeof(int));
    *newsockfd = accept(*sockfd, (struct sockaddr *) &cli_addr, &clilen);
    checkError(newsockfd, "ERROR on accepting", "Accepted");

    // initRecvSession(&newsockfd);
    state = pthread_create(&tid, NULL, initRecvSession, (void *)newsockfd);
    if (state){
        printf("ERROR; return code from pthread_create() is %d\n", state);
        exit(-1);
    }

    // close socket
    // close(*sockfd);
}

void *initRecvSession(void *param) {
    int *newsockfd = (int*)param;
    char buffer[BUFFER_SIZE], httphead[BUFFER_SIZE];
    int state;
    // while (true) {


        // receive message from the client to buffer
        // memset(&buffer, 0, sizeof(buffer));
        // state = recv(*newsockfd, buffer, BUFFER_SIZE, 0);

        // printf("%s\n", get_handshake_key("Hello"));

        open_handshake(newsockfd);
        checkError(newsockfd, "handshaking failed", "handshaking succeed");

        processMessage(buffer);
        searchCommand(buffer);

        // send message to the client
        // memset(&httphead, 0, sizeof(httphead));
        // state = send(*newsockfd, httphead, strlen(httphead), 0);
        // checkError(newsockfd, "ERROR on accepting", "Accepted");

    // }
    pthread_exit(NULL);
}

void checkError(int *sockfd, char *errormsg, char *successmsg) {
    if (sockfd < 0) {
        perror(errormsg);
        exit(1);
    }
    printf("-- %s --\n", successmsg);

}

int processMessage(char *message) {
    // if (*message != '=') {
    //     pthread_exit(NULL);
    //     printf("Connection lost\n");
    //     return -1;
    // }
    // message++;
    if (*message == '/') {
        searchCommand(message);
        return 1;
    } else {
        printf("Here is the message: %s", message);
        return 0;
    }
}

void searchCommand(char *command) {
    if (strcmp(command, "/exit") == 0) {
        printf("exited\n");
        pthread_exit(NULL);
    }
}
