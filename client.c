#define BUFFER_SIZE 512
#define NUM_THREADS 8

int initSocket(char *host, char *portno);
void initConnection(int *sockfd);
void *initRecvSession(void *param);
int processMessage(char *message);
void searchCommand(char *command);

#include "include/helper.h"
#include "include/chat.h"
#include "include/websocket.h"

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

int main(int argc, char *argv[]) {
    // declaration
    int sockfd, portno, error;
    int len = 256;
    struct sockaddr_in serv_addr;
    char buffer[len];
    struct hostent *server;

    // command line argument
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    portno = atoi(argv[2]);

    // socket // int socket (int family, int type, int protocol);
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // init sockaddr_in structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno); // htons converts from host byte order to network byte order
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // inet_addr convert dot notation into network address in Network Byte Order
    // bcopy((char *)&serv_addr.sin_addr.s_addr,
    //       (char *)server->h_addr,
    //       server->h_length);

    // connect // connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
    error = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (error < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    // message from user to be sent
    printf("Please enter the message: ");
    memset(buffer, 0, 256);
    fgets(buffer, 255, stdin);

    // send/write
    error = send(sockfd, buffer, strlen(buffer), 0);
    if (error < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    // recieve/read
    memset(buffer, 0, 256);
    error = recv(sockfd, buffer, len, 0);
    printf("Here is the message: %s\n", buffer);
    if (error < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    http_frame dataframe;
    uint64_t header;

    dataframe.opcode = 129;
    dataframe.mask = 0;
    dataframe.payloadlen = 4;
    dataframe.payload[0] = 'H';
    header = dataframe.opcode;
    header = header | dataframe.mask << 8;
    header = header | dataframe.payloadlen << 9;
    header = header | dataframe.payload[0] << 16;
    header = header | dataframe.payload[0] << 17;
    header = header | dataframe.payload[0] << 18;
    header = header | dataframe.payload[0] << 19;
    send(sockfd, &header, sizeof(header), 0);

    printf("%s\n", buffer);

    close(sockfd);
    return 0;
}
