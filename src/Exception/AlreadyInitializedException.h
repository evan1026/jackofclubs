#ifndef ALREADY_INITIALIZED_EXCEPTION_H
#define ALREADY_INITIALIZED_EXCEPTION_H

#include "Exception/Exception.h"

class AlreadyInitializedException : public Exception {
public:
    AlreadyInitializedException(int stack_skip = 0) : Exception("AlreadyInitializedException: Class has already been initialized and cannot exist twice", stack_skip) {}
};

#endif
