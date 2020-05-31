#pragma once

#include <cstdlib>

/*!
 * Hashing class so that enums can be used as
 * a key for various stl containers (e.g. map)
 */
struct EnumClassHash {
    /*! Function that does the conversion so that the hashing works. */
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};
