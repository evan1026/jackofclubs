#ifndef NOT_YET_INITIALIZED_EXCEPTION_H
#define NOT_YET_INITIALIZED_EXCEPTION_H

#include "Exception/Exception.h"

class NotYetInitializedException : public Exception {
public:
    NotYetInitializedException(int stack_skip = 0) : Exception("NotYetInitializedException: Class has not yet been initialized", stack_skip) {}
};

#endif
