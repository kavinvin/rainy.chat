/**
  @file  socket.c
  @brief A header file include server socket
*/

#include "socket.h"

int initSocket(char *host, char *portno) {
    int sockfd, state;
    struct sockaddr_in server_address;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    showStatus("Creating socket");
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return -1;
    }

    // create server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(portno)); // htons converts from host byte order to network byte order
    server_address.sin_addr.s_addr = inet_addr(host); // inet_addr convert dot notation into network address in Network Byte Order

    // bind the socket to the server address
    showStatus("Binding");
    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("ERROR on binding");
        return -1;
    }

    // listening to the socket
    showStatus("Listening");
    if (listen(sockfd, 5)) {
        perror("ERROR on listening");
        return -1;
    }

    return sockfd;
}
