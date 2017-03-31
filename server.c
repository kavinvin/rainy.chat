/**
  @file  server.c
  @brief Instant messaging API
*/

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
    printf("sockfd closed\n");
    close(*sockfd);
}

void *initRecvSession(void *param) {
    int *newsockfd = (int*)param;
    char buffer[BUFFER_SIZE], message[BUFFER_SIZE];
    int state;

    http_frame dataframe;
    uint64_t header;

    // open_handshake(newsockfd);
    checkError(newsockfd, "handshaking failed", "handshaking succeed");

    // while (1) {
        // receive message from the client to buffer
        memset(&buffer, 0, sizeof(buffer));
        state = recv(*newsockfd, buffer, BUFFER_SIZE, 0);
        checkError(newsockfd, "Error on recieving message", "Message received");
        // processMessage(buffer);
        printf("Here is the message: %s\n", buffer);


        printBits(sizeof(buffer), buffer);
        // printf("\n");
        // printf("\n");
        // printBits(sizeof(dataframe), &dataframe);
        // printf("\n");
        // printf("\n");

        // prepare dataframe
        dataframe.opcode = 129;
        dataframe.mask = 0;
        dataframe.payloadlen = 113;
        dataframe.payload[0] = 'H';
        dataframe.payload[1] = 'e';
        dataframe.payload[2] = 'l';
        dataframe.payload[3] = 'o';
        header = dataframe.opcode;
        header = header | dataframe.mask << 8;
        header = header | dataframe.payloadlen << 9;
        header = header | dataframe.payload[0] << 16;
        header = header | dataframe.payload[1] << 17;
        header = header | dataframe.payload[2] << 18;
        header = header | dataframe.payload[3] << 19;

        // send message to the client
        // memset(&message, 0, sizeof(message));
        // strcpy(message, "Hello\n");
        // state = send(*newsockfd, (void *)&dataframe, strlen(buffer), 0);
        // checkError(newsockfd, "Error on sending message", "Message sent");
    // }

    close(*newsockfd);
    printf("newsockfd closed\n");
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
