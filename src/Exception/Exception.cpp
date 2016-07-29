#include <cstring>
#include <cxxabi.h>
#include <execinfo.h>
#include <sstream>

#include "Exception/Exception.h"

#define STRING_BUFFER_SIZE 1024

void Exception::demangle(std::stringstream& ss, char** symbollist, size_t addrlen) const {
    size_t funcnamesize = STRING_BUFFER_SIZE;
    char funcname[STRING_BUFFER_SIZE];

    // iterate over the returned symbol lines
    // stack skip is how many to skip because is takes some extra function
    // calls to get from where the throw is done to where the backtrace was saved
    // addrlen - 2 is because the first 2 are above main and not really useful to us
    for (unsigned int i = _stackSkip; i < addrlen - 2; i++) {
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

            static char buf[STRING_BUFFER_SIZE];
            char* p = end_offset;

            while (*p != ']') {
                p++;
            }

            *p = '\0';

            // prepare command to be executed
            // our program need to be passed after the -e parameter
            sprintf (buf, "/usr/bin/addr2line -e ./jackofclubs %s", end_offset + 2);
            FILE* f = popen (buf, "r");
            fgets (buf, STRING_BUFFER_SIZE, f);

            ss << "    " << fname << " - ";
            if (buf[0] != '?') {
                ss << buf; // buf already has newline
            } else {
                ss << "unknown location" << std::endl;
            }

            pclose(f);
        } else {
            // couldn't parse the line? print the whole line.
            ss << "    " << symbollist[i] << std::endl;
        }
    }
}

void Exception::printStackTrace(std::stringstream& ss) const {
    char** lines = backtrace_symbols(_stackTrace, _stackSize);
    demangle(ss, lines, _stackSize);
    delete[] lines;
}

void Exception::saveStackTrace() {
    _stackSize = backtrace(_stackTrace, STACK_TRACE_MAX_SIZE);
}

Exception::Exception(std::string reason, int stackSkip) :
            std::runtime_error(reason), _reason(reason), _stackSkip(stackSkip + 3) { //+3 because of the 3 extra functions we add
    _out = new char*;
    saveStackTrace();
}

const char* Exception::what() const noexcept {
    std::stringstream ss;
    ss << _reason << std::endl;
    printStackTrace(ss);

    *_out = new char[ss.str().length()]; //Have to do it this way to get around the const thing, plus I want it to be freed in the destructor
    strcpy(*_out, ss.str().c_str());
    return *_out;
}

