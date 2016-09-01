#ifndef NULL_MAYBE_EXCEPTION_H
#define NULL_MAYBE_EXCEPTION_H

#include "Exception/Exception.h"

class NullMaybeException : public Exception {
public:
    NullMaybeException(int stack_skip = 0) : Exception("NullMaybeException: Attempted to turn an empty maybe into a value.", stack_skip) {}
};

#endif
