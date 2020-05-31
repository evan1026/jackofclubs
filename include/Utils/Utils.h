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

    /*!
     * Gets the width of an sf::Text
     */
    static float textWidth(const sf::Text& text) {
        sf::Vector2f start = text.findCharacterPos(0);
        sf::Vector2f end   = text.findCharacterPos(std::numeric_limits<std::size_t>::max());
        return end.x - start.x;
    }
};
