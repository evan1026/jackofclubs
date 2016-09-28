#include <iostream>
#include <signal.h>

#include "Exception/NullptrException.h"
#include "Exception/SegmentationFaultException.h"
#include "Game.h"

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
        std::cout << "Warning: Could not register segfault handler" << std::endl;
        std::cout << "If a segmentation fault occurs, no stack trace will be available." << std::endl << std::endl;
    }

    try {
        return doMain();
    } catch (std::exception& e) {
        std::cout << std::endl;
        std::cout << "***************************************" << std::endl;
        std::cout << "* Exception uncaught. Must terminate. *" << std::endl;
        std::cout << "*      Exception details follow       *" << std::endl;
        std::cout << "***************************************" << std::endl;
        std::cout << e.what() << std::endl;
        std::cout << "If you're seeing this in production, please screenshot and send to the developer." << std::endl;
        return 1;
    }
}

