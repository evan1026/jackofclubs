#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

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
