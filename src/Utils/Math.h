#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <type_traits>

/*!
 *
 * A collection of math functions
 *
 * To keep all math in the same place, I've
 * redefined a number of stl functions to
 * reside in here
 */
namespace Math {
    /*! Way more digits of pi than anyone should ever need */
    constexpr double PI = 3.1415926535897932384626433832795;

    /*! \callergraph
     *
     * Template signum implementation from
     * http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
     *
     * Returns -1 if x < 0     <br>
     * Returns  1 if x > 0     <br>
     * Returns  0 if x == 0    <br>
     */
    template <typename T> inline constexpr
    int signum(T x, std::false_type is_signed) {
        return T(0) < x;
    }

    /*! \callergraph
     *
     * Template signum implementation from
     * http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
     *
     * Returns -1 if x < 0     <br>
     * Returns  1 if x > 0     <br>
     * Returns  0 if x == 0    <br>
     */
    template <typename T> inline constexpr
    int signum(T x, std::true_type is_signed) {
        return (T(0) < x) - (x < T(0));
    }

    /*! \callergraph
     *
     * Template signum implementation from
     * http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
     *
     * Returns -1 if x < 0     <br>
     * Returns  1 if x > 0     <br>
     * Returns  0 if x == 0    <br>
     */
    template <typename T> inline constexpr
    int signum(T x) {
        return signum(x, std::is_signed<T>());
    }

    /*! \callergraph
     * Returns the absolute value of x
     */
    template <typename T> inline constexpr
    T abs(T x) {
        return (x < 0) ? -x : x;
    }

    /*! \callergraph
     * Truncates x to [places] decimal places
     */
    template <typename T> inline constexpr
    T trunc(T x, int places) {
        return std::floor(x * std::pow(10, places)) / pow(10, places);
    }

    /*! \callergraph
     * Returns the greatest integer lower than x
     */
    template <typename T> inline constexpr
    T floor(T x) {
        return std::floor(x);
    }

    /*! \callergraph
     * Returns the lowest integer greater than x
     */
    template <typename T> inline constexpr
    T ceil(T x) {
        return std::ceil(x);
    }

    /*! \callergraph
     * Rounds x to the nearest integer
     */
    template <typename T> inline constexpr
    T round(T x) {
        return std::round(x);
    }

    /*! \callergraph
     * Returns the square root of x
     */
    template <typename T> inline constexpr
    T sqrt(T x) {
        return std::sqrt(x);
    }

    /*! \callergraph
     * Returns true if x and y are the same up to [places] decimal places
     */
    template <typename T> inline constexpr
    bool similar(T x, T y, int places) {
        return trunc(abs(x - y), places) == T(0);
    }

    /*! \callergraph
     * Returns the sine of x (radians)
     */
    template <typename T> inline constexpr
    double sin(T x) {
        return std::sin(x);
    }

    /*! \callergraph
     * Returns the cosine of x (radians)
     */
    template <typename T> inline constexpr
    double cos(T x) {
        return std::cos(x);
    }

    /*! \callergraph
     * Returns the sine of x (degrees)
     */
    template <typename T> inline constexpr
    double sinDeg(T x) {
        return std::sin(x * PI / 180.f);
    }

    /*! \callergraph
     * Returns the cosine of x (degrees)
     */
    template <typename T> inline constexpr
    double cosDeg(T x) {
        return std::cos(x * PI / 180.f);
    }

    /*! \callergraph
     * Returns the minimum of a collection of values
     */
    template <typename T> inline constexpr
    T min(T x, T y) {
        return (x < y) ? x : y;
    }

    /*! \callergraph
     * Returns the minimum of a collection of values
     */
    template <typename T, typename... Ts> inline
    T min(T x, T y, Ts... rest) {
        T minOfRest = min(y, rest...);
        return (x < minOfRest) ? x : minOfRest;
    }

    /*! \callergraph
     * Returns the maximum of a collection of values
     */
    template <typename T> inline constexpr
    T max(T x, T y) {
        return (x > y) ? x : y;
    }

    /*! \callergraph
     * Returns the maximum of a collection of values
     */
    template <typename T, typename... Ts> inline
    T max(T x, T y, Ts... rest) {
        T maxOfRest = max(y, rest...);
        return (x > maxOfRest) ? x : maxOfRest;
    }
};

#endif
