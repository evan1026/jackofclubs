#ifndef NULLPTR_EXCEPTION_H
#define NULLPTR_EXCEPTION_H

#include "Exception/Exception.h"

/*
 * Exception thrown if someone tries to dereference a null pointer.
 * NOTE: this is not actually part of the language. This can only be
 * thrown if you check to see if the pointer is null and throw if it is.
 * Without checking, a segfault with occur, just as normal.
 *
 * TODO: use in segfault handler
 */

class NullptrException : public Exception {
public:
    NullptrException(int stack_skip = 0) : Exception("NullptrException: Tried to dereference null pointer", stack_skip + 1) {}
};

#endif
