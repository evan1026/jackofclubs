#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

/*
 * My own font class which is the same as sf::Font
 * except that it defines a default font
 */

class Font : public sf::Font {

    public:
        static Font defaultFont;

        Font() {
            loadFromFile("resources/arial.ttf");
        }

        Font(std::string file) {
            loadFromFile(file);
        }
};

#endif
