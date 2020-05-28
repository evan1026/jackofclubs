#ifndef SEGMENTATION_FAULT_EXCEPTION_H
#define SEGMENTATION_FAULT_EXCEPTION_H

#include <sstream>

#include "Exception/Exception.h"

/*!
 * Exception thrown if someone tries to dereference an invalid pointer (not null)
 */
class SegmentationFaultException : public Exception {

    std::string generateMessage(void *invalidLocation) {
        std::stringstream ss;
        ss << "SegmentationFaultException: Tried to access invalid memory at address " << invalidLocation;
        return ss.str();
    }

public:
    /*! \callergraph
     * Constructor
     */
    SegmentationFaultException(void* loc, void* address, int stack_skip = 0) : Exception(generateMessage(loc), stack_skip + 1, address) {}
};

#endif
