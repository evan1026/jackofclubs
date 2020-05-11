#ifndef MAYBE_H
#define MAYBE_H

#include "Exception/NullMaybeException.h"

/*!
 * This class is based off of the class of the same name in Haskell.
 * It represents a value that may or may not exist. A Maybe of any
 * given type holds a pointer to a value, which will either point
 * to actual data or to nothing. Care should be taken to ensure
 * that the Maybe actually stores a value before trying to use it.
 *
 * Example usage:
 *
 *     int a;
 *     Maybe<int> maybeWithValue(10);
 *     Maybe<int> maybeWithoutValue();
 *
 *     if (maybeWithValue) {
 *         //there is data, using it is safe
 *         a = maybeWithValue();
 *     } else {
 *         //will not get here, since it has data
 *     }
 *
 *     if (maybeWithoutValue) {
 *         //will not get here, since it has no data
 *     } else {
 *         //this will throw a NullMaybeException
 *         a = maybeWithoutValue();
 *     }
 *
 *     //you can also assign directly from a value
 *     maybeWithoutValue = 10;
 *
 *     //Or from a null pointer
 *     maybeWithValue = nullptr;
 *
 *     if (maybeWithValue) {
 *         //it will no longer make it here, since it no longer
 *         //has a value
 *     }
 *
 *     if(maybeWithoutValue) {
 *         //It will now make it in here, since it now has a value
 *     }
 */
template <typename T>
class Maybe {

    T* value;
    std::allocator<T> alloc;

public:
    /*! \callergraph
     * Constructs a non-empty Maybe by forwarding the args to the
     * templated type's constructor.
     */
    template <typename... Ts>
    Maybe<T>(const Ts&... args) {
        value = alloc.allocate(1);
        alloc.construct(value, args...);
    }

    /*! \callergraph
     * Constructs an empty maybe
     */
    Maybe<T>() {
        value = nullptr;
    }

    /*! \callergraph
     * Frees up the contained value, if there is one
     */
    ~Maybe<T>() {
        if (value != nullptr) {
            alloc.destroy(value);
        }
    }

    /*! \callergraph
     * Copy constructor
     */
    Maybe<T>(const Maybe<T>& other) {
        if (other.value != nullptr) {
            value = alloc.allocate(1);
            alloc.construct(value, *other);
        } else {
            value = nullptr;
        }
    }

    /*! \callergraph
     * Move constructor
     */
    Maybe<T>(Maybe<T>&& other) {
        value = other.value;
        other.value = nullptr;
    }

    /*! \callergraph
     * Copy assignment operator
     */
    Maybe<T>& operator=(const Maybe<T>& other) {
        if (value != nullptr) {
            alloc.destroy(value);
        }

        if (other.value != nullptr) {
            value = alloc.allocate(1);
            alloc.construct(value, *other);
        } else {
            value = nullptr;
        }

        return *this;
    }

    /*! \callergraph
     * Move assignment operator
     */
    Maybe<T>& operator=(Maybe<T>&& other) {
        if (value != nullptr){
            alloc.destroy(value);
        }

        value = other.value;
        other.value = nullptr;

        return *this;
    }

    /*! \callergraph
     * Assign to Maybe based on anything T can be assigned by
     */
    template <typename O>
    Maybe<T>& operator=(const O& other) {
        if (value == nullptr) {
            value = alloc.allocate(1);
        }

        *value = other;

        return *this;
    }

    /*! \callergraph
     * Assign will null pointer
     */
    Maybe<T>& operator=(const std::nullptr_t& null_p) {
        if (value != nullptr) {
            alloc.destroy(value);
        }

        value = null_p;

        return *this;
    }

    /*! \callergraph
     * Checks if value can be extracted from this Maybe
     */
    operator bool() const {
        return value != nullptr;
    }

    /*! \callergraph
     * Extracts the value from this Maybe, and throws and exception if we can't
     */
    T& operator()() const {
        if (*this) return *value;
        else throw NullMaybeException();
    }
};

#endif