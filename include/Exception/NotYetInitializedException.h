#ifndef NOT_YET_INITIALIZED_EXCEPTION_H
#define NOT_YET_INITIALIZED_EXCEPTION_H

#include "Exception/Exception.h"

/*!
 * Exception thrown when a class is used before it was initialized.
 */
class NotYetInitializedException : public Exception {
public:
    /*! \callergraph
     * Constructor
     */
    NotYetInitializedException(int stack_skip = 0) : Exception("NotYetInitializedException: Class has not yet been initialized", stack_skip + 1) {}
};

#endif
