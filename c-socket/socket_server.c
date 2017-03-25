#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void unbind(int *sockfd) {
    int error, non_zero = 1;
    error = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &non_zero, sizeof(int));
    if (error < 0) {
        printf("setsockopt(SO_REUSEADDR) failed\n");
    }
}

void check_error(int *sockfd, char *errormsg) {
    if (sockfd < 0) {
        unbind(sockfd);
        perror(errormsg);
        exit(1);
    }
}

int listen_for_connection(char *host, char *port) {
    int sockfd, newsockfd, portno, error;
    int len = 256;
    socklen_t clilen;
    char buffer[len];
    struct sockaddr_in serv_addr, cli_addr;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    check_error(&sockfd, "ERROR opening socket");

    // init sockaddr_in structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port)); // htons converts from host byte order to network byte order
    serv_addr.sin_addr.s_addr = inet_addr(host); // inet_addr convert dot notation into network address in Network Byte Order

    /* Now bind the host address using bind() call.*/
    error = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (error < 0) {
        perror("ERROR on binding");
        return newsockfd;
    }
    printf("binded\n");

    // listen
    listen(sockfd, 5);
    printf("listened\n");

    // accept
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    printf("accepted\n");

    // recieve/read
    error = recv(newsockfd, buffer, len, 0);
    printf("Here is the message: %s\n", buffer);
    if (error < 0) {
        perror("ERROR reading from socket");
        return newsockfd;
    }
    printf("recieved\n");

    // send/write
    error = send(newsockfd, "message recieved", 30, 0);

    if (error < 0) {
        perror("ERROR writing to socket");
        return newsockfd;
    }

    close(sockfd);
    return newsockfd;
}

int main(int argc, char *argv[]) {
    listen_for_connection(argv[1], argv[2]);
}

