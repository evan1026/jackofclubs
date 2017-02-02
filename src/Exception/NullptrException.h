#ifndef NULLPTR_EXCEPTION_H
#define NULLPTR_EXCEPTION_H

#include "Exception/Exception.h"

/*!
 * Exception thrown if someone tries to dereference a null pointer.
 */
class NullptrException : public Exception {
public:
    /*! \callergraph
     * Constructor
     */
    NullptrException(int stack_skip = 0) : Exception("NullptrException: Tried to dereference null pointer", stack_skip + 1) {}
};

#endif
