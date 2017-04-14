/**
  @file  server.c
  @brief Instant messaging API
*/

#include "error.h"
#include "helper.h"

void sig_handler(int sig) {
    if (sig == SIGSEGV) {
        printlog("Segmentation fault\n");
    }
    if (sig == SIGINT || sig == SIGSEGV) {
        // free memory
        // if (l != NULL) {
        //     list_free(l);
        //     l = NULL;
        // }
        signal(sig, SIG_DFL);
        printlog("\nFreeing memory...\n");
        printlog("Exiting...\n");
        exit(0);
    } else if (sig == SIGPIPE) {
        signal(sig, SIG_IGN);
    }
}
