#include "headers/chat.h"
#include "headers/helper.h"

void checkError(int *sockfd, char *errormsg, char *successmsg);
int initSocket(char *host, char *port);
void initConnection(int *sockfd);

int main(int argc, char *argv[]) {
    int sockfd;
    sockfd = initSocket(argv[1], argv[2]);
    initConnection(&sockfd);
}

int initSocket(char *host, char *port) {
    int sockfd, state;
    struct sockaddr_in server_address;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    checkError(&sockfd, "ERROR opening socket", "Socket created");

    // create server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(port)); // htons converts from host byte order to network byte order
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
    int newsockfd, state;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in cli_addr;
    socklen_t clilen;

    // accept incoming request, create new client socket
    clilen = sizeof(cli_addr);
    newsockfd = accept(*sockfd, (struct sockaddr *) &cli_addr, &clilen);
    checkError(&newsockfd, "ERROR on accepting", "Accepted");

    // receive message from the client to buffer
    state = recv(newsockfd, buffer, BUFFER_SIZE, 0);
    checkError(&state, "ERROR reading from socket", "Message received");
    printf("Here is the message: %s\n", buffer);

    // send message to the client
    state = send(newsockfd, "Message received", 16, 0);
    checkError(&state, "ERROR writing to socket", "Message sent");

    // close socket
    close(*sockfd);
}

void checkError(int *sockfd, char *errormsg, char *successmsg) {
    if (sockfd < 0) {
        perror(errormsg);
        exit(1);
    }
    printf("%s\n", successmsg);

}
