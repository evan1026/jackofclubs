#include <iostream>
#include <signal.h>

#include "Exception/NullptrException.h"
#include "Exception/SegmentationFaultException.h"
#include "Game.h"
#include "Logger/Logger.hpp"

Logger::Logger _log;

/*
 * Program starts here.
 * Registers signal handlers and starts the game running. Also
 * prints out details about any exceptions left uncaught.
 */

int doMain(){

    Game g;
    g.run();

    return 0;
}

void segvHandler(int sig, siginfo_t* si, void* unused) {

    // Throw exception. The 2 tells it to skip the top two functions (which are the OS's function
    // for handling the trap the CPU generates and this function)
    if (si->si_addr == nullptr) {
        throw NullptrException(2);
    } else {
        throw SegmentationFaultException(si->si_addr, 2);
    }
}

int main() {

    // Registers the segfault handler
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segvHandler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        _log.warn("Could not register segfault handler");
        _log.warn("If a segmentation fault occurs, no stack trace will be available.\n");
    }

    try {
        return doMain();
    } catch (std::exception& e) {
        _log.error();
        _log.error("***************************************");
        _log.error("* Exception uncaught. Must terminate. *");
        _log.error("*      Exception details follow       *");
        _log.error("***************************************");
        _log.error();

        std::stringstream exceptionDetails(e.what());
        std::string line;
        while(getline(exceptionDetails, line)) {
            _log.error(line);
        }

        _log.error();
        _log.error();
        _log.error("If you're seeing this in production, please screenshot and send to the developer.");
        return 1;
    }
}

