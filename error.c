/**
 * File: error.c
 * ----------------------------
 *   define error and signal handler
 */

#include "error.h"
#include "helper.h"

void sig_handler(int sig) {
    if (sig == SIGSEGV) {
        printlog("Segmentation fault\n");
    }
    if (sig == SIGINT || sig == SIGSEGV) {
        signal(sig, SIG_DFL);
        printlog("\nFreeing memory...\n");
        printlog("Exiting...\n");
        exit(0);
    } else if (sig == SIGPIPE) {
        signal(sig, SIG_IGN);
    }
}
