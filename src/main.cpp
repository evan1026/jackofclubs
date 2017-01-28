#include <iostream>
#include <fstream>

#include "Exception/NullptrException.h"
#include "Exception/SegmentationFaultException.h"
#include "Game.h"
#include "Logger/GlobalLogger.hpp"
#include "Platforms.h"

#ifdef JOC_LINUX_BUILD
#include <signal.h>
#endif

#ifdef JOC_WINDOWS_BUILD
#include "windows.h"
#endif

/*
 * Program starts here.
 * Registers signal handlers and starts the game running. Also
 * prints out details about any exceptions left uncaught.
 */

using Logger::globalLogger;

int doMain(){

    globalLogger.log("Welcome to Jack of Clubs!");
    globalLogger.log("This game is in development, so make sure to send bug reports to the author (evan1026 on github)");
    globalLogger.log();

    Game g;
    g.run();

    return 0;
}

void registerSegfaultHandler();

int main() {

    std::ofstream logFile("log.txt");
    globalLogger.addStream(logFile, false);

    // Registers the segfault handler
    registerSegfaultHandler();

    try {
        return doMain();
    } catch (std::exception& e) {
        globalLogger.error();
        globalLogger.error("***************************************");
        globalLogger.error("* Exception uncaught. Must terminate. *");
        globalLogger.error("*      Exception details follow       *");
        globalLogger.error("***************************************");
        globalLogger.error();

        std::stringstream exceptionDetails(e.what());
        std::string line;
        while(getline(exceptionDetails, line)) {
            globalLogger.error(line);
        }

        globalLogger.error();
        globalLogger.error();
        globalLogger.error("If you're seeing this in production, please screenshot and send to the developer.");
        return 1;
    }
}

#ifdef JOC_LINUX_BUILD
void segvHandler(int sig, siginfo_t* si, void* unused) {

    // Throw exception. The 2 tells it to skip the top two functions (which are the OS's function
    // for handling the trap the CPU generates and this function)
    if (si->si_addr == nullptr) {
        throw NullptrException(2);
    } else {
        throw SegmentationFaultException(si->si_addr, 2);
    }
}

void registerSegfaultHandler() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segvHandler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        globalLogger.warn("Could not register segfault handler");
        globalLogger.warn("If a segmentation fault occurs, no stack trace will be available.\n");
    }
}
#endif

#ifdef JOC_WINDOWS_BUILD
LONG WINAPI segvHandler(EXCEPTION_POINTERS* einfo) {
    void* address = nullptr;
    switch (einfo->ExceptionRecord->ExceptionCode) {
        case EXCEPTION_ACCESS_VIOLATION:
            // Throw exception. The 2 tells it to skip the top two functions (which are the OS's function
            // for handling the trap the CPU generates and this function)
            address = (void*)einfo->ExceptionRecord->ExceptionInformation[1];
            if (address == nullptr) {
                throw NullptrException(2);
            } else {
                throw SegmentationFaultException(address, 2);
            }
            break;
        default:
            break;
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

void registerSegfaultHandler() {
    SetUnhandledExceptionFilter(segvHandler);
}
#endif
