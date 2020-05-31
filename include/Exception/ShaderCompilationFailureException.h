#pragma once

#include <sstream>

#include "Exception/Exception.h"

/*!
 * Exception thrown if someone tries to dereference an invalid pointer (not null)
 */
class ShaderCompilationFailureException : public Exception {

    std::string generateMessage(std::string fileName, std::string errorMessage) {
        std::stringstream ss;
        ss << "ShaderCompilationFailureException: Failed to compile " << fileName << ": " << errorMessage;
        return ss.str();
    }

public:
    /*! \callergraph
     * Constructor
     */
    ShaderCompilationFailureException(std::string fileName, std::string errorMessage, int stack_skip = 0) : Exception(generateMessage(fileName, errorMessage), stack_skip + 1) {}
};
