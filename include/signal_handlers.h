#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H


#include <signal.h>

#define SIGNAL_TABLE

typedef void (*sighandler_t)(int);

sighandler_t Signal(int signum, sighandler_t handler);


SIGNAL_TABLE void handle_sigchld(int signum);







#endif