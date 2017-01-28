#ifdef linux
#include <cstring>
#include <cxxabi.h>
#include <execinfo.h>
#include <sstream>

#include "Exception/Exception.h"
#include "Logger/GlobalLogger.hpp"

#define STRING_BUFFER_SIZE 1024

using Logger::globalLogger;

/*
* "Demangles" stack trace item names.
* Code from https://panthema.net/2008/0901-stacktrace-demangled/
*
* When C++ code is translated into assembly, each function has to be given
* a label to jump to. With C code, that label can just be the function name.
* However, with C++, the full function name would be something like
* Exception::demangle (and that doesn't even take function overloading into account,
* but I have no idea how they handle that). Since the colon (:) isn't a valid
* character to be part of a label, they have to "mangle" the name into a format
* that is valid. The format it turns into is pretty much unreadable. For instance,
* this function becomes:
*     _ZNK9Exception8demangleERNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEEPPcm
*
* Which is a lot less readable than the header below. So, this function takes that
* mess and translates it back using a provided function abi::__cxa_demangle
*
* ss         - The string stream representing the final output that we'll write into
* symbollist - array of strings containing the mangled names
* addrlen    - number of entries to process (name is a carry-over from where I got this code)
*/
void Exception::demangle(std::stringstream& ss, char** symbollist, size_t addrlen) const {
    size_t funcnamesize = STRING_BUFFER_SIZE;
    char funcname[STRING_BUFFER_SIZE];

    // iterate over the returned symbol lines
    // stack skip is how many to skip because is takes some extra function
    // calls to get from where the throw is done to where the backtrace was saved
    // addrlen - 2 is because the first 2 are above main and not really useful to us
    for (unsigned int i = _stackSkip; i < addrlen - 2; i++) {
        char* begin_name = NULL;
        char* begin_offset = NULL;
        char* end_offset = NULL;

        for (char *p = symbollist[i]; *p; ++p) {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && (begin_offset || begin_name))
                end_offset = p;
        }

        if (begin_name && end_offset && (begin_name < end_offset)) {
            *begin_name++ = '\0';
            *end_offset++ = '\0';
            if (begin_offset)
                *begin_offset++ = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():

            int status = 0;
            char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
            char* fname = begin_name;
            if (status == 0)
                fname = ret;

            static char buf[STRING_BUFFER_SIZE];
            char* p = end_offset;

            while (*p != ']') {
                p++;
            }

            *p = '\0';

            // prepare command to be executed
            // our program need to be passed after the -e parameter
            sprintf(buf, "/usr/bin/addr2line -e ./jackofclubs %s", end_offset + 2);
            FILE* f = popen(buf, "r");
            fgets(buf, STRING_BUFFER_SIZE, f);

            ss << "    " << fname << " - ";
            if (buf[0] != '?') {
                ss << buf; // buf already has newline
            }
            else {
                ss << "unknown location" << std::endl;
            }

            pclose(f);
        }
        else {
            // couldn't parse the line? print the whole line.
            ss << "    " << symbollist[i] << std::endl;
        }
    }
}

/*
* Prints the stacktrace saved in the exception into the stringstream.
*
* ss - the stringstream to print the data into
*/
void Exception::printStackTrace(std::stringstream& ss) const {
    char** lines = backtrace_symbols(_stackTrace, _stackSize);
    demangle(ss, lines, _stackSize);
    delete[] lines;
}

/*
* Uses linux-specific library function to save the stacktrace for this function call.
* The fact that it will include everything up to this function is why we have the
* stack skip later. There's no point in printing stuff for the Exception library itself.
*/
void Exception::saveStackTrace() {
    _stackSize = backtrace(_stackTrace, STACK_TRACE_MAX_SIZE);
}


#endif