#pragma once

#include <SFML/Graphics.hpp>

/*!
 * My own font class which is the same as sf::Font
 * except that it defines a default font
 */
class Font : public sf::Font {

    public:
        /*! The font to use by default so you don't have to make one yourself */
        static Font defaultFont;

        /*! Constructs a font from the default location ("resources/arial.ttf") */
        Font() {
            loadFromFile("resources/arial.ttf");
        }

        /*! Constructs a font from the location given */
        Font(std::string file) {
            loadFromFile(file);
        }
};
