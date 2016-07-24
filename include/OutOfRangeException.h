#ifndef OUT_OF_RANGE_EXCEPTION_H
#define OUT_OF_RANGE_EXCEPTION_H

#include "Exception.h"

class OutOfRangeException : public Exception {
public:
    OutOfRangeException(int stack_skip = 0) : Exception("OutOfRangeException: tried to reference object outside of range of container", stack_skip) {}
};

#endif
