#pragma once

#include "Graphics/Menu/Menu.h"

class {NAME}Menu : public Menu {

    protected:
        void renderMenu(sf::RenderWindow& w) override;

    public:
        {NAME}Menu();

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;
        bool handleKeyPressed(const sf::Event::KeyEvent& e) override;
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleResize(const sf::Event::SizeEvent& e) override;
};
