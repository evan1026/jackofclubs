#pragma once

#include "Exception/Exception.h"

/*!
 * Exception thrown when attempting to access a member that is out of
 * range of a container. Specificly for this project; does nothing about
 * stl containers.
 */
class OutOfRangeException : public Exception {
public:
    /*! \callergraph
     * Constructor
     */
    OutOfRangeException(int stack_skip = 0) : Exception("OutOfRangeException: tried to reference object outside of range of container", stack_skip + 1) {}
};
