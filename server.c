/**
  @file  server.c
  @brief Instant messaging API
*/

#define BUFFERSIZE 1024
#define NUM_THREADS 8

int initSocket(char *host, char *portno);
void initConnection(int *sockfd);
void *initRecvSession(void *param);
int processMessage(char *message);
void searchCommand(char *command);

#include "include/helper.h"
#include "include/chat.h"
#include "include/websocket.h"

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
    checkError(sockfd,
               "ERROR opening socket",
               "Socket created");

    // create server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(portno)); // htons converts from host byte order to network byte order
    server_address.sin_addr.s_addr = inet_addr(host); // inet_addr convert dot notation into network address in Network Byte Order

    // bind the socket to the server address
    checkError(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)),
               "ERROR on binding",
               "Socket binded");

    // listening to the socket
    checkError(listen(sockfd, 5),
               "ERROR on listening",
               "Listening");

    return sockfd;
}

void initConnection(int *sockfd) {
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

        // initRecvSession(&newsockfd);
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
    char buffer[BUFFERSIZE], message[BUFFERSIZE];
    http_frame frame;

    checkError(open_handshake(newsockfd),
               "handshaking failed",
               "handshaking succeed");

    while (1) {
        // receive message from client
        memset(&frame, 0, sizeof(frame));
        ws_recv(newsockfd, &frame);
        printf("Here is the message from no.%d: %s\n", *newsockfd, frame.payload);

        // send message to client
        memset(&frame, 0, sizeof(frame));
        frame.opcode = 129;
        frame.has_mask = 0;
        frame.payload = "Hello!";
        frame.len = strlen(frame.payload);
        ws_send(newsockfd, &frame);
    }

    close(*newsockfd);
    printf("newsockfd no.%d closed\n", *newsockfd);
    pthread_exit(NULL);
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
        printf("Here is the message: %s\n", message);
        return 0;
    }
}

void searchCommand(char *command) {
    if (strcmp(command, "/exit") == 0) {
        printf("exited\n");
        pthread_exit(NULL);
    }
}
