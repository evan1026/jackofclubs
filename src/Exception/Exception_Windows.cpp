#ifdef _WIN32

#include "Exception/Exception.h"

void Exception::demangle(std::stringstream& ss, char** symbollist, size_t addrlen) const {
    return;
}

void Exception::printStackTrace(std::stringstream& ss) const {
    return;
}

void Exception::saveStackTrace() {
    return;
}

#endif