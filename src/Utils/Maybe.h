#ifndef MAYBE_H
#define MAYBE_H

#include "Exception/NullMaybeException.h"

/*********************************************************************
 * ~~~~~~~~~~~~                                                      *
 * ~ Maybe<T> ~                                                      *
 * ~~~~~~~~~~~~                                                      *
 *                                                                   *
 * This class is based off of the class of the same name in Haskell. *
 * It represents a value that may or may not exist. A Maybe of any   *
 * given type holds a pointer to a value, which will either point    *
 * to actual data or to nothing. Care should be taken to ensure      *
 * that the Maybe actually stores a value before trying to use it.   *
 *                                                                   *
 * Example usage:                                                    *
 *     int a;                                                        *
 *     Maybe<int> maybeWithValue(10);                                *
 *     Maybe<int> maybeWithoutValue();                               *
 *                                                                   *
 *     if (maybeWithValue) {                                         *
 *         //there is data, using it is safe                         *
 *         a = maybeWithValue();                                     *
 *     } else {                                                      *
 *         //will not get here, since it has data                    *
 *     }                                                             *
 *                                                                   *
 *     if (maybeWithoutValue) {                                      *
 *         //will not get here, since it has no data                 *
 *     } else {                                                      *
 *         //this will throw a NullMaybeException                    *
 *         a = maybeWithoutValue();                                  *
 *     }                                                             *
 *                                                                   *
 *     //you can also assign directly from a value                   *
 *     maybeWithoutValue = 10;                                       *
 *                                                                   *
 *     //Or from a null pointer                                      *
 *     maybeWithValue = nullptr;                                     *
 *                                                                   *
 *     if (maybeWithValue) {                                         *
 *         //it will no longer make it here, since it no longer      *
 *         //has a value                                             *
 *     }                                                             *
 *                                                                   *
 *     if(maybeWithoutValue) {                                       *
 *         //It will now make it in here, since it now has a value   *
 *     }                                                             *
 *********************************************************************/
template <typename T>
class Maybe {

    T* value;
    std::allocator<T> alloc;

public:
    // Automatically handles things like copy and move constructors (for T),
    // because it just forwards things along
    template <typename... Ts>
    Maybe<T>(const Ts&... args) {
        value = alloc.allocate(1);
        alloc.construct(value, args...);
    }

    Maybe<T>() {
        value = nullptr;
    }

    ~Maybe<T>() {
        if (value != nullptr) {
            alloc.destroy(value);
        }
    }

    Maybe<T>(const Maybe<T>& other) {
        value = alloc.allocate(1);
        if (other.value != nullptr) {
            *value = *other.value;
        } else {
            value = nullptr;
        }
    }

    Maybe<T>(Maybe<T>&& other) {
        value = other.value;
        other.value = nullptr;
    }

    template <typename... Ts>
    Maybe<T>& operator=(const Ts&... args) {
        if (value != nullptr) {
            alloc.destroy(value);
        }
        value = alloc.allocate(1);
        alloc.construct(value, args...);
        return *this;
    }

    Maybe<T>& operator=(const Maybe<T>& other) {
        if (value != nullptr) {
            alloc.destroy(value);
        }
        value = alloc.allocate(1);
        if (other.value != nullptr) {
            *value = *other.value;
        } else {
            value = nullptr;
        }
        return *this;
    }

    Maybe<T>& operator=(Maybe<T>&& other) {
        if (value != nullptr){
            alloc.destroy(value);
        }
        value = other.value;
        other.value = nullptr;
        return *this;
    }

    Maybe<T>& operator=(const std::nullptr_t& null_p) {
        if (value != nullptr) {
            alloc.destroy(value);
        }
        value = null_p;
        return *this;
    }

    operator bool() const {
        return value != nullptr;
    }

    T& operator()() const {
        if (*this) return *value;
        else throw NullMaybeException();
    }
};

#endif