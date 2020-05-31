#pragma once

#include "Game.h"
#include "Graphics/Components/Button.h"
#include "Graphics/Menu/Menu.h"

class EscapeMenu : public Menu {

    Game& _game;

    Button _quitButton;
    Button _resumeButton;

    bool _resumeButtonClicked;

    void buttonCallback(const std::string& s);

    protected:
        void renderMenu(sf::RenderWindow& w) override;

    public:
        EscapeMenu(int width, int height, Game& g);

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;
        bool handleKeyPressed(const sf::Event::KeyEvent& e) override;
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleResize(const sf::Event::SizeEvent& e) override;
};
