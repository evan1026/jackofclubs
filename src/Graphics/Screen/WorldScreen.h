#ifndef WORLD_SCREEN_H
#define WORLD_SCREEN_H

#include <chrono>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Debug/Graphics/FPSCounter.h"
#include "Graphics/Menu/Menu.h"
#include "Graphics/Screen/Screen.h"
#include "Player.h"
#include "Rendering/RenderEngine.h"
#include "World/BlockFace.h"
#include "World/World.h"

class Game;

/*
 * This is the big class that does most of the work for displaying the world
 * to the user and letting them interact with it.
 *
 * For docs on each function, see WorldScreen.cpp
 */

class WorldScreen : public Screen {

    //Rendered objects
    World _world;
    Player _player; // Not *rendered* in the traditional sense, but we call render() on it to handle translation
    FPSCounter _fpsCounter; // Only displayed in debug mode
    std::unique_ptr<Menu> _activeMenu;
    sf::RectangleShape _colorRect;
    sf::RectangleShape _centerRect; // Basically a mouse cursor for the user

    //Other data
    bool _mouseCaptured;
    sf::Vector2i _screenMiddle;
    sf::Color _selectedColor;
    Maybe<BlockFace> _selectedBlock;

    // References to other objects we need to work with
    // I'm sure there's a better way to do this, but this is easier and
    // shouldn't cause any issues
    sf::RenderWindow& _window;
    Game& _game;

    void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& event);
    void handleKeyPressed(const sf::Event::KeyEvent& event);
    void handleMouseMoved(const sf::Event::MouseMoveEvent& event);
    void handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e);

    void placeBlock();
    void removeBlock();

    void toggleColorSelectorMenu();
    void removeMenu();

    void handlePlayerMovement();

    void copySelectionColor();

    public:
        WorldScreen(sf::RenderWindow& window, Game& game);

        void handleEvent(const sf::Event& event) override;
        void tick() override;

        void render(RenderEngine& re, sf::RenderWindow& w) override;
};

#endif
