#ifndef ALREADY_INITIALIZED_EXCEPTION_H
#define ALREADY_INITIALIZED_EXCEPTION_H

#include "Exception/Exception.h"

/*
 * Exception thrown when trying to initialize a class that has already been initialized,
 * and cannot be initialized twice. See Exception.h for more documentation on exceptions.
 */

class AlreadyInitializedException : public Exception {
public:
    AlreadyInitializedException(int stack_skip = 0) : Exception("AlreadyInitializedException: Class has already been initialized and cannot exist twice", stack_skip + 1) {}
};

#endif
