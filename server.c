/**
  @file  server.c
  @brief Instant messaging API
*/

#include "headers/chat.h"
#include "headers/helper.h"

void checkError(int *sockfd, char *errormsg, char *successmsg);
int initSocket(char *host, char *portno);
void initConnection(int *sockfd);
void *initRecvSession(void *param);
void searchCommand(char *command);

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
    char buffer[BUFFER_SIZE];
    int state;
    while (true) {
        // receive message from the client to buffer
        memset(&buffer, 0, sizeof(buffer));
        state = recv(*newsockfd, buffer, BUFFER_SIZE, 0);
        checkError(&state, "ERROR reading from socket", "Message received");
        searchCommand(buffer);
        printf("Here is the message: %s\n", buffer);

        // send message to the client
        state = send(*newsockfd, "Message received", 16, 0);
        if (state < 0) {
            printf("Connection lost\n");
            exit(1);
        }
    }
    pthread_exit(NULL);
}

void checkError(int *sockfd, char *errormsg, char *successmsg) {
    if (sockfd < 0) {
        perror(errormsg);
        exit(1);
    }
    printf("%s\n", successmsg);

}

void searchCommand(char *command) {
    if (strcmp(command, "/exit") == 0) {
        pthread_exit(NULL);
    }
}
