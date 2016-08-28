#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <SFML/Graphics.hpp>

struct Utils {
    template <typename T> inline
    static std::string toString(T x) {
        static std::stringstream ss;

        ss.str("");
        ss << x;
        return ss.str();
    }

    static float textWidth(const sf::Text& text) {
        sf::Vector2f start = text.findCharacterPos(0);
        sf::Vector2f end   = text.findCharacterPos(std::numeric_limits<std::size_t>::max());
        return end.x - start.x;
    }
};

#endif
