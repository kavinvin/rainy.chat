/**
 * File: server.c
 * ----------------------------
 *   an executable file of main functionalities
 */

#include "server.h"

/**
 * Function: serveRainyChat
 * ----------------------------
 *   serveRainyChat(host, port)
 *   start chat application server on the given host and port
 *   return 0 if success, -1 if failed
 */
int serveRainyChat(char *host, char *port) {
    int server_socket;

    printlog("-- Server started --\n");

    // create CTRL-C and Segmentation faults signal handler
    signal(SIGINT, &signalHandler);
    signal(SIGSEGV, &signalHandler);

    // create server socket
    server_socket = initSocket(host, port);

    // create users list
    List *all_users = newList();

    // prepare mutex
    initMutex(3, &all_users->lock, &mutex_log, &mutex_accept);

    // create threads serving each client
    forkService(server_socket, all_users);

    return 0;

}

/**
 * Function: parseAddr
 * ----------------------------
 *   parse host and port information from command line argument
 *   if not found, request them from stdin
 *   return void
 */
void parseAddr(int argc, char *argv[], char **host, char **port) {
    // ask for host and port if argument not found
    if (argc <= 1) {
        *host = calloc(1, 20); // *memory leaked
        *port = calloc(1, 10); // *memory leaked
        fputs("Please specify host and port to serve Rainy.Chat\n", stdout);
        fputs("Host: ", stdout);
        fgets(*host, 20, stdin);
        fputs("Port: ", stdout);
        fgets(*port, 10, stdin);
    //  get host and port from argument
    } else {
        *host = argv[1];
        *port = argv[2];
    }
}

/**
 * Function: newList
 * ----------------------------
 *   create double-circular linked list structure
 *   return new list pointer
 */
List *newList(void) {
    List *list;
    list = malloc(sizeof(list));
    list->len = 0;
    list->head = NULL;
    return list;
}

/**
 * Function: initMutex
 * ----------------------------
 *   init all of the given mutex
 *   return 0 if success, -1 if failed
 */
int initMutex(int count, ...) {
    va_list args;
    va_start(args, count);
    for (int i=0; i<count; i++) {
        if (pthread_mutex_init( va_arg(args, pthread_mutex_t*), NULL ) != 0) {
            printlog("Error on mutex init: %s\n", strerror(errno));
            return -1;
        }
    }
    va_end(args);
    return 0;
}

/**
 * Function: forkService
 * ----------------------------
 *   create new thread for each new client
 *   return void
 */
void forkService(int server_socket, List *all_users) {
    int state;

    // create argument struct for passing to new thread
    pthread_args_t args;
    args.server_socket = server_socket;
    args.list = all_users;

    // thread settings
    pthread_t *thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (1) {

        // lock mutex before accepting new user
        thread_id = malloc(sizeof(pthread_t));
        pthread_mutex_lock(&mutex_accept);

        // create new thread
        state = pthread_create(thread_id, NULL, initRecvSession, (void*)&args);
        if (state){
            printlog("ERROR: return code from pthread_create() is %d\n", state);
            free(thread_id);
        }
    }

    // destory pthread attibute
    pthread_attr_destroy(&attr);

}

/**
 * Function: initRecvSession
 * ----------------------------
 *   start new thread serving each client
 *   return void
 */
void *initRecvSession(void *param) {
    // convert void pointer to arguments
    pthread_args_t *args = (pthread_args_t*)param;
    int server_socket = args->server_socket;
    List *all_users = args->list;

    // users list structure
    User *user;
    Node *this;

    // message buffer for communication
    char *message;
    http_frame frame;

    // accept new user and unlock mutex
    user = acceptUser(server_socket);

    // web browser handshaking
    user->header = openHandshake(user->socket);
    if (user->header == NULL) {
        removeUser(user);
        pthread_exit(NULL);
    }

    // create new user node
    this = create(user);
    if (this == NULL) {
        printlog("Error on creating node\n");
        removeUser(user);
        pthread_exit(NULL);
    }

    // validate whether a user can join a chat room
    while (user->credit) {
        if (validateUser(all_users, this, &frame) == 0) {
            break;
        }
        user->credit--;
    }

    // check user remaining credit
    // if less than 0, cut the connection
    if (!user->credit) {
        removeNode(all_users, this);
        pthread_exit(NULL);
    }

    // append user to chatroom
    append(all_users, this);

    // send online status to all user
    sendStatus(all_users, user, NULL);

    while (1) {
        // receive message from the user
        message = getMessage(all_users, this, &frame);

        // broadcast message to all users
        broadcast(all_users, this, message, OTHER);
        free(message);
    }

    removeNode(all_users, this);
    pthread_exit(NULL);
}

/**
 * Function: acceptUser
 * ----------------------------
 *   allocate memory for a new user and accept their connection
 *   return user struct
 */
User *acceptUser(int server_socket) {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    // create user: should test if successfully create a user
    User *user = malloc(sizeof(User));
    if (!user) {
        printlog("Error on memory: %s\n", strerror(errno));
        pthread_exit(NULL);
    }

    // accept incoming request, create new client socket
    user->socket = accept(server_socket, (struct sockaddr *) &cli_addr, &clilen);
    if (user->socket < 0) {
        printlog("Error on accepting: %s\n", strerror(errno));
        free(user);
        exit(1);
    }

    // unlock mutex, ready for new client
    pthread_mutex_unlock(&mutex_accept);
    printlog("-- Accepting client --\n");

    // set up default attibute
    user->ip_address = inet_ntoa(cli_addr.sin_addr);
    user->thread_id = pthread_self();
    user->name = NULL;
    user->credit = 20;

    return user;
}

/**
 * Function: validateUser
 * ----------------------------
 *   ask for user preriquisite before joining room
 *   return 0 if success, -1 if failied
 */
int validateUser(List *all_users, Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    json_t* json;
    json_error_t json_err;
    Node *cursor;
    User *otheruser;

    // recieve login information
    memset(frame, 0, sizeof(*frame));
    if (wsRecv(this, frame) != SUCCESS) {
        removeUser(user);
        pthread_exit(NULL);
    }

    // extract json string to a structure
    json = json_loads(frame->message, 0, &json_err);
    if (json == NULL) {
        printlog("Login error: invalid json\n");
        broadcast(all_users, this, "{\"type\":\"login\",\"iserror\":1,\"errormsg\":\"Invalid format\"}", SELF);
        free(json);
        free(frame->message);
        return -1;
    }

    // extract username from json
    json_unpack(json, "{s:s}", "username", &user->name);
    if (user->name == NULL) {
        printlog("Login error: no usernmae given\n");
        broadcast(all_users, this, "{\"type\":\"login\",\"iserror\":1,\"errormsg\":\"No username given\"}", SELF);
        free(json);
        free(frame->message);
        return -1;
    }

    // check if the username has been taken
    cursor = all_users->head;
    if (cursor != NULL) {
        do {
            otheruser = (User*)cursor->data;
            if (strcmp(user->name, otheruser->name) == 0) {
                printlog("Login error: username taken\n");
                broadcast(all_users, this, "{\"type\":\"login\",\"iserror\":1,\"errormsg\":\"Username taken\"}", SELF);
                free(json);
                free(frame->message);
                return -1;
            }
            cursor = cursor->next;
        } while (cursor != all_users->head);
    }

    printf("Username: %s\n", user->name);

    // notify all users of the new user
    broadcast(all_users, this, "{\"type\":\"login\",\"iserror\":0}", SELF);
    free(json);
    free(frame->message);
    return 0;
}

/**
 * Function: getMessage
 * ----------------------------
 *   listen for a message from client
 *   return 0 if success, -1 if failied
 */
char *getMessage(List *all_users, Node *this, http_frame *frame) {
    User *user = (User*)this->data;
    char *message;
    json_t *json;
    json_error_t json_err;

    // receive message from user
    memset(frame, 0, sizeof(*frame));
    if (wsRecv(this, frame) != SUCCESS) {
        removeNode(all_users, this);
        pthread_exit(NULL);
    };
    readMessage(all_users, this, frame->message); // mutex

    // prepare broadcasting data to all users in the room
    json = json_pack("{s:i, s:s, s:s, s:s}",
                     "id", user->socket,
                     "type", "message",
                     "username", user->name,
                     "message", frame->message);
    message = json_dumps(json, JSON_COMPACT);
    free(json);
    free(frame->message);
    return message;
}

/**
 * Function: initServerSession
 * ----------------------------
 *   listen for server command
 *   return void
 */
void *initServerSession(void *server_socket_param) {
    int *server_socket = (int*)server_socket_param;
    char command[20];
    while (1) {
        fgets(command, 20, stdin);
        serverCommand(server_socket, command);
    }
    pthread_exit(NULL);
}

/**
 * Function: readMessage
 * ----------------------------
 *   read a message received from client
 *   return 0 if it's a command, 1 if it's a text message
 */
int readMessage(List *all_users, Node *this, char *message) {
    User *user = (User*)this->data;
    if (*message == '/') {
        // command mode
        clientRequest(all_users, this, message);
        return 0;
    } else {
        // message mode
        printlog("Message received from #%d: %s (%s)\nMessage: %s\n",
                 user->socket,
                 user->name,
                 user->ip_address,
                 message);
        return 1;
    }
}

/**
 * Function: clientRequest
 * ----------------------------
 *   search for available command for the user
 *   return void
 */
void clientRequest(List *all_users, Node *this, char *command) {
    if (strcmp(command, "/exit") == 0) {
        printlog("Exited\n");
        removeNode(all_users, this);
        pthread_exit(NULL);
    } else {
        printlog("Client command not found\n");
    }
}

/**
 * Function: serverCommand
 * ----------------------------
 *   search for available command for the server
 *   return void
 */
void serverCommand(int *server_socket, char *command) {
    printlog("got command\n");
    if (strcmp(command, "/exit\n") == 0) {
        printlog("Shutting down...\n");
        // while (head != NULL) {
        //     removeNode(head->prev);
        // }
        close(*server_socket);
        printlog("Socket Closed\n");
        exit(0);
    } else {
        printlog("Server command not found\n");
    }
}
