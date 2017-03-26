#include "chat.h"

void check_error(int *sockfd, char *errormsg, char *successmsg) {
    if (sockfd < 0) {
        perror(errormsg);
        exit(1);
    }
    printf("%s\n", successmsg);

}

int init_socket(char *host, char *port) {
    int sockfd, newsockfd, portno, state;
    int len = 256;
    socklen_t clilen;
    char buffer[len];
    struct sockaddr_in serv_addr, cli_addr;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    check_error(&sockfd, "ERROR opening socket", "Socket created");

    // init sockaddr_in structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port)); // htons converts from host byte order to network byte order
    serv_addr.sin_addr.s_addr = inet_addr(host); // inet_addr convert dot notation into network address in Network Byte Order

    /* Now bind the host address using bind() call.*/
    state = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    check_error(&state, "ERROR on binding", "Socket binded");

    // listen
    state = listen(sockfd, 5);
    check_error(&state, "ERROR on listening", "Listening");

    // accept
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    check_error(&newsockfd, "ERROR on accepting", "Client accecpted");

    // receive/read
    state = recv(newsockfd, buffer, len, 0);
    printf("Here is the message: %s\n", buffer);
    check_error(&state, "ERROR reading from socket", "Message recieved");

    // send/write
    state = send(newsockfd, "Message received", 30, 0);
    check_error(&state, "ERROR writing to socket", "Message sent");

    // close socket
    close(sockfd);
    return newsockfd;
}

int main(int argc, char *argv[]) {
    init_socket(argv[1], argv[2]);
}
