#pragma once

#include <sstream>
#include <stdexcept>

#define STACK_TRACE_MAX_SIZE 500

/*!
 * Base class for all exceptions. It resembles Java's Exception
 * in that it will save the stacktrace and will print it out as
 * part of Exception::what
 */
class Exception : public std::runtime_error {
    char ** _out;                            // The text to use in what(). It's a pointer to a pointer so that the text can be mutable, even when the exception is const
    std::string _reason;                     // The reason the exception was thrown. Basically, eveything that will be printed but the stack trace
    void *_stackTrace[STACK_TRACE_MAX_SIZE]; // Data representing the stack trace. Note: not the strings
    size_t _stackSize;                       // Actual size of the stack trace
    int _stackSkip;                          // How much of the stack trace to skip (see constructor below)
    void* _address;

    void printStackTrace(std::stringstream& ss) const;
    void demangle(std::stringstream&ss, char** symbollist, size_t addrlen) const;
    void saveStackTrace();

public:

    // See Exception.cpp for a description of this function
    Exception(std::string reason, int stackSkip, void* address = nullptr);

    /*!
     * Handles freeing output string. This way, whoever I send it to doesn't have to worry about it
     *
     * NOTE: If planning to make use of the text outside the lifetime of this object, please save it
     * somewhere else (preferably in std::string, to avoid memory issues entirely)
     */
    virtual ~Exception() {
        if (*_out != nullptr) delete[] *_out;
        delete _out;
    }

    // Overrides std::runtime_error::what()
    // Automatically called by runtime environment if exception is left uncaught
    const char * what() const noexcept override;
};
