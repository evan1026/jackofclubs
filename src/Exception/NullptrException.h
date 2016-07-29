#ifndef NULLPTR_EXCEPTION_H
#define NULLPTR_EXCEPTION_H

#include "Exception/Exception.h"

class NullptrException : public Exception {
public:
    NullptrException(int stack_skip = 0) : Exception("NullptrException: Tried to dereference null pointer", stack_skip) {}
};

#endif
