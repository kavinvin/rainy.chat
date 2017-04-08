/**
  @file  server.c
  @brief Instant messaging API
*/

#include "server.h"

int main(int argc, char *argv[]) {
    int sockfd;
    char *host = argv[1];
    char *port = argv[2];
    pthread_t server_thread;
    node_count = 0;

    // init mutex
    pthread_mutex_init(&mutex_head, NULL);

    sockfd = initSocket(host, port);
    if (sockfd < 0) {
        exit(1);
    }
    // pthread_create(&server_thread, NULL, initServerSession, (void*)&sockfd);
    initClient(&sockfd);
    close(sockfd);
    pthread_exit(NULL);
}

void initClient(int *sockfd) {
    int state;
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    pthread_t *thread_id;
    pthread_attr_t attr;
    User *user;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (1) {

        // create user: should test if successfully create a user
        user = malloc(sizeof(User));

        // accept incoming request, create new client socket
        user->socket = accept(*sockfd, (struct sockaddr *) &cli_addr, &clilen);
        printf("\n------------------------------------------------------------\n");
        showStatus("Accepting client");
        if (user->socket < 0) {
            free(user);
            perror("ERROR on accepting");
            continue;
        }

        printf("Client socket id: %d\n", user->socket);

        thread_id = malloc(sizeof(pthread_t));

        state = pthread_create(thread_id, NULL, initRecvSession, (void*)user);
        showStatus("Creating new thread");
        if (state){
            printf("ERROR: return code from pthread_create() is %d\n", state);
            close(user->socket);
            free(thread_id);
            free(user);
        }

    }

    pthread_attr_destroy(&attr);

}

void *initRecvSession(void *user_param) {
    User *user = (User*)user_param;
    Node *this;
    char *message;
    http_frame frame;
    json_t* json;
    json_error_t error;
    char *roomname;

    // assign temporary username
    user->thread_id = pthread_self();

    if (open_handshake(user->socket) < 0) {
        perror("handshaking failed");
        removeUser(user);
        pthread_exit(NULL);
    }

    // create node
    this = create(user);
    if (this == NULL) {
        printf("%s\n", "Error on creating node");
        removeUser(user);
        pthread_exit(NULL);
    }

    // prerequisite: username and roomname
    memset(&frame, 0, sizeof(frame));
    ws_recv(this, &frame); // mutex
    json = json_loads(frame.message, 0, &error);
    json_unpack(json, "{s:s, s:s}", "username", &user->name, "roomname", &roomname);
    free(json);

    // insert node
    pthread_mutex_lock(&mutex_head);
    insert(head, this);
    head = this;
    pthread_mutex_unlock(&mutex_head);

    while (1) {
        // receive message from client
        memset(&frame, 0, sizeof(frame));
        ws_recv(this, &frame); // mutex
        parseMessage(this, frame.message); // mutex

        // build json
        json = json_pack("{s:s, s:s}", "username", user->name, "message", frame.message);
        message = json_dumps(json, JSON_COMPACT);
        free(json);
        free(frame.message);

        // send json to client
        memset(&frame, 0, sizeof(frame));
        frame.opcode = 129;
        frame.message = message;
        frame.size = strlen(frame.message);
        // protect from receiving message more than 1200 characters
        if (frame.size > 1200) {
            removeNode(this);
            pthread_exit(NULL);
        }
        map(this, broadcast, &frame); // mutex
        free(frame.message);
    }

    removeNode(this);
    pthread_exit(NULL);
}

void *initServerSession(void *sockfd_param) {
    int *sockfd = (int*)sockfd_param;
    char command[20];
    while (1) {
        fgets(command, 20, stdin);
        serverCommand(sockfd, command);
    }
    pthread_exit(NULL);
}

int parseMessage(Node *this, char *message) {
    User *user = (User*)this->data;
    if (*message == '/') {
        // command mode
        clientRequest(this, message);
        return 0;
    } else {
        // message mode
        printf("Here is the message from no.%d: %s\n", user->socket, message);
        return 1;
    }
}

void clientRequest(Node *this, char *command) {
    if (strcmp(command, "/exit") == 0) {
        printf("Exited\n");
        removeNode(this);
        pthread_exit(NULL);
    } else {
        printf("Client command not found\n");
    }
}

void serverCommand(int *sockfd, char *command) {
    printf("got command\n");
    if (strcmp(command, "/exit\n") == 0) {
        printf("Shutting down...\n");
        // while (head != NULL) {
        //     removeNode(head->prev);
        // }
        close(*sockfd);
        printf("Socket Closed\n");
        exit(0);
    } else {
        printf("Server command not found\n");
    }
}
