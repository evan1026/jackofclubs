#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

class Font : public sf::Font {

    public:
        static Font defaultFont;

        Font() {
            loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf"); //TODO temp location... should be moved into project
        }

        Font(std::string file) {
            loadFromFile(file);
        }
};

#endif
