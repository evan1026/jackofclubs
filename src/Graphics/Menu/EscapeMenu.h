#ifndef ESCAPE_MENU_H
#define ESCAPE_MENU_H

#include "Graphics/Menu/Menu.h"

class EscapeMenu : public Menu {

    protected:
        void renderMenu(sf::RenderWindow& w) override;

    public:
        EscapeMenu(int width, int height);

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;
        bool handleKeyPressed(const sf::Event::KeyEvent& e) override;
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleResize(const sf::Event::SizeEvent& e) override;
};

#endif
