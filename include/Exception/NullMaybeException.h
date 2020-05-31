#pragma once

#include "Exception/Exception.h"

/*!
 * Exception thrown when someone tries to extract a value from an empty Maybe.
 * See Utils/Maybe.h for more info.
 */
class NullMaybeException : public Exception {
public:
    /*! \callergraph
     * Constructor
     */
    NullMaybeException(int stack_skip = 0) : Exception("NullMaybeException: Attempted to turn an empty maybe into a value.", stack_skip + 1) {}
};
