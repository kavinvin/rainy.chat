/**
 * File: main.c
 * ----------------------------
 *   everything starts here
 */
#include "server.h"

int main(int argc, char *argv[])
{
    char *host, *port;

    // parse socket address from command line arguments
    parseAddr(argc, argv, &host, &port);

    // start chat application
    serveRainyChat(host, port);

    return 0;
}


