#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <type_traits>

namespace Math {
    // Template signum implementation from
    // http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
    template <typename T> inline constexpr
    static int signum(T x, std::false_type is_signed) {
        return T(0) < x;
    }

    template <typename T> inline constexpr
    static int signum(T x, std::true_type is_signed) {
        return (T(0) < x) - (x < T(0));
    }

    template <typename T> inline constexpr
    static int signum(T x) {
        return signum(x, std::is_signed<T>());
    }


    template <typename T> inline constexpr
    static T abs(T x) {
        return (x < 0) ? -x : x;
    }

    //truncates to <places> decimal places
    template <typename T> inline constexpr
    static T trunc(T x, int places) {
        return std::floor(x * std::pow(10, places)) / pow(10, places);
    }

    template <typename T> inline constexpr
    static bool similar(T x, T y, int places) {
        return trunc(abs(x - y), places) == T(0);
    }
};

#endif
