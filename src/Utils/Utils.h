#ifndef UTILS_H
#define UTILS_H

#include <sstream>

struct Utils {
    template <typename T> inline
    static std::string toString(T x) {
        static std::stringstream ss;

        ss.str("");
        ss << x;
        return ss.str();
    }
};

#endif
