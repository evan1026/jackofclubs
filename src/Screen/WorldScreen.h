#ifndef WORLD_SCREEN_H
#define WORLD_SCREEN_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Player.h"
#include "Screen/Screen.h"
#include "World/World.h"

class Game;

class WorldScreen : public Screen {

    World _world;
    Player _player;
    bool _mouseCaptured;
    sf::Vector2i _screenMiddle;
    sf::Window& _window;
    Game& _game;

    void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& event);
    void handleKeyPressed(const sf::Event::KeyEvent& event);

    public:
        WorldScreen(sf::Window& window, Game& game);

        void handleEvent(const sf::Event& event) override;
        void tick() override;
};

#endif
