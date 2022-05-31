#ifndef SIGHANDLER_LIST_H
#define SIGHANDLER_LIST_H

#include <signal_handlers.h>

sighandler_t signal_handler_list[] = {handle_sigchld};
int signum_list[] = {SIGCHLD};
int num_sighandlers = 1;

#endif