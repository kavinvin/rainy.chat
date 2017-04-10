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
    printLog("Creating socket\n");
    if (sockfd < 0) {
        printLog("ERROR on opening socket: %s\n", strerror(errno));
        return -1;
    }

    // create server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(portno)); // htons converts from host byte order to network byte order
    server_address.sin_addr.s_addr = inet_addr(host); // inet_addr convert dot notation into network address in Network Byte Order

    // bind the socket to the server address
    printLog("Binding\n");
    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printLog("ERROR on binding: %s\n", strerror(errno));
        return -1;
    }

    // listening to the socket
    printLog("Listening\n");
    if (listen(sockfd, 5)) {
        printLog("ERROR on listening: %s\n", strerror(errno));
        return -1;
    }

    return sockfd;
}
