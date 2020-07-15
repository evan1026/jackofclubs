#pragma once

#include <sstream>

#include <SFML/Graphics.hpp>

/*!
 * Defines a number of miscellaneous utility functions
 */
struct Utils {

    /*!
     * Converts an object to a string based on its << operator
     */
    template <typename T> inline
    static std::string toString(T x) {
        static std::stringstream ss;

        ss.str("");
        ss << x;
        return ss.str();
    }

};
