#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <sstream>
#include <stdexcept>
#include <execinfo.h>

class Exception : public std::runtime_error {
    char ** _out;
    std::string _reason;
    void *_stackTrace[500];
    size_t _stackSize;
    int _stackSkip;

    void printStackTrace(std::stringstream& ss, int stack_skip) const;
    void demangle(std::stringstream&ss, char** symbollist, size_t addrlen, int stack_skip) const;
    void saveStackTrace();

public:


    //Stack skip should be essentially the number of classes in between the original class and this one
    //
    //ex: class ExceptionA : public Exception should use stack_skip 0
    //    but class ExceptionB : public ExceptionA should use stack_skip 1
    //    (increase 1 every time the class heirarchy goes one level deeper)
    Exception(std::string reason, int stackSkip);
    virtual ~Exception() {
        if (*_out != nullptr) delete[] *_out;
        delete _out;
    }

    const char * what() const noexcept;
};
#endif
