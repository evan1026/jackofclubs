// Including both and putting the if statements in the files makes Visual Studio happier
// Doesn't really affect the code too much so eh
#include "Exception/Exception_Windows.cpp"
#include "Exception/Exception_Linux.cpp"

#include <cstring>

#include "Exception/Exception.h"
#include "Logger/GlobalLogger.hpp"

using Logger::globalLogger;

/*! \callergraph
 *
 * Constructs a new Exception.
 *
 * The stack skip should be essentially how many function calls exist between the original
 * exception constructor and this one, which basically boils down to the class's level in the
 * heirarchy (i.e., direct descendants of this class should give 1, their children give 2, etc.).
 * As such, it probably makes sense to have each guy take an optional stack skip and call this
 * constructor with that value plus 1.
 *
 * \p reason    - The issue that caused the exception itself. Mostly in the format "ExceptionName: Cause of exception."        <br>
 * \p stackSkip - explained above                                                                                              <br>
 * \p address   - The address in the code that caused the exception. Defaults to nullptr. Only used for segmentation faults    <br>
 */
Exception::Exception(std::string reason, int stackSkip, void* address) :
            std::runtime_error(reason), _reason(reason), _stackSkip(stackSkip + 2), _address(address) { //+2 because of the 2 extra functions we add (constructor and printStackTrace())
    globalLogger.warn("Exception thrown - ", reason);
    _out = new char*;
    *_out = nullptr;
    saveStackTrace();
}

/*! \callergraph
 *
 * Returns a readable explaination of the exception.
 * Overrides std::runtime_error::what().
 *
 * This is where the backtrace is translated into text.
 * I delay it like that because creating a string representation involves
 * getting line numbers, which involves calling /usr/bin/addr2line a bunch
 * of times. If an exception is thrown and later caught without printing
 * anything, there's no reason to invoke multiple instances of another process.
 */
const char* Exception::what() const noexcept {
    std::stringstream ss;
    ss << _reason << std::endl;
    printStackTrace(ss);

    *_out = new char[ss.str().length()]; //Have to do it this way to get around the const thing, plus I want it to be freed in the destructor
    strcpy(*_out, ss.str().c_str());
    return *_out;
}
