#ifndef MATH_H
#define MATH_H

#include <type_traits>

class Math {
    public:

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
};

#endif
