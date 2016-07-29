#include <cstring>
#include <cxxabi.h>
#include <execinfo.h>
#include <sstream>
#include <stdexcept>

#include "Exception/Exception.h"

void Exception::demangle(std::stringstream& ss, char** symbollist, size_t addrlen, int stack_skip) const {
    size_t funcnamesize = 1024;
    char funcname[1024];

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (unsigned int i = stack_skip + 1; i < addrlen - 2; i++) {
        char* begin_name   = NULL;
        char* begin_offset = NULL;
        char* end_offset   = NULL;

        for ( char *p = symbollist[i]; *p; ++p ) {
            if ( *p == '(' )
                begin_name = p;
            else if ( *p == '+' )
                begin_offset = p;
            else if ( *p == ')' && ( begin_offset || begin_name ))
                end_offset = p;
        }

        if ( begin_name && end_offset && ( begin_name < end_offset )) {
            *begin_name++   = '\0';
            *end_offset++   = '\0';
            if ( begin_offset )
                *begin_offset++ = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():

            int status = 0;
            char* ret = abi::__cxa_demangle( begin_name, funcname, &funcnamesize, &status );
            char* fname = begin_name;
            if ( status == 0 )
                fname = ret;

            static char buf[256];
            char* p = end_offset;

            while (*p != ']') {
                p++;
            }

            *p = '\0';

            // prepare command to be executed
            // our program need to be passed after the -e parameter
            sprintf (buf, "/usr/bin/addr2line -e ./jackofclubs %s", end_offset + 2);
            FILE* f = popen (buf, "r");

            fgets (buf, 256, f);

            if (buf[0] != '?') {
                ss << "    " << fname << " - " << buf;
            } else {
                ss << "    " << fname << " - unknown location" << std::endl;
            }

            pclose(f);
        } else {
            // couldn't parse the line? print the whole line.
            ss << "    " << symbollist[i] << std::endl;
        }
    }
}

void Exception::printStackTrace(std::stringstream& ss, int stackSkip) const {
    char** lines = backtrace_symbols(_stackTrace, _stackSize);
    demangle(ss, lines, _stackSize, stackSkip);
    delete[] lines;
}

void Exception::saveStackTrace() {
    _stackSize = backtrace(_stackTrace, 1000);
}

Exception::Exception(std::string reason, int stackSkip) :
            std::runtime_error(reason), _reason(reason), _stackSkip(stackSkip + 2) { //+2 because of the 2 extra functions we add
    _out = new char*;
    saveStackTrace();
}

const char* Exception::what() const noexcept {
    std::stringstream ss;
    ss << _reason << std::endl;
    printStackTrace(ss, _stackSkip);

    *_out = new char[ss.str().length()]; //Have to do it this way to get around the const thing
    strcpy(*_out, ss.str().c_str());
    return *_out;
}

