#pragma once

#include "Game.h"
#include "Graphics/Components/Button.h"
#include "Graphics/Menu/Menu.h"

class EscapeMenu : public Menu {

    Game& _game;
    Screen& _screen;

    std::shared_ptr<Button> _quitButton;
    std::shared_ptr<Button> _resumeButton;
    std::shared_ptr<Button> _settingsButton;

    bool _resumeButtonClicked;

    void buttonCallback(const std::string& s);

    public:
        EscapeMenu(int width, int height, Game& g, Screen& screen);
        virtual ~EscapeMenu() = default;

        virtual void layout(const sf::RenderWindow& w) override;

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleResize(const sf::Event::SizeEvent& e) override;
};
