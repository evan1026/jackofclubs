#pragma once

#include <memory>

#include <SFML/System.hpp>

#include "Debug/Graphics/FPSCounter.h"
#include "Graphics/Menu/Menu.h"
#include "Graphics/Screen/Screen.h"
#include "Player.h"
#include "Utils/Events/IKeyboardEventHandler.h"
#include "Utils/Events/IMouseEventHandler.h"
#include "World/World.h"

class Game;

/*!
 * This is the big class that does most of the work for displaying the world
 * to the user and letting them interact with it.
 */
class WorldScreen : public Screen, public IKeyboardEventHandler, public IMouseEventHandler {

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

    // References to other objects we need to work with
    // I'm sure there's a better way to do this, but this is easier and
    // shouldn't cause any issues
    sf::RenderWindow& _window;
    Game& _game;

    bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& event) override;
    bool handleKeyPressed(const sf::Event::KeyEvent& event) override;
    bool handleMouseMoved(const sf::Event::MouseMoveEvent& event) override;
    bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;

    void placeBlock();
    void removeBlock();

    void addMenu(Menu* const m) override;
    void removeMenu() override;

    void handlePlayerMovement();

    void copySelectionColor();

    public:
        WorldScreen(sf::RenderWindow& window, Game& game);

        bool handleEvent(const sf::Event& event) override;
        void tick() override;

        void render(RenderEngine& re, sf::RenderWindow& w) override;
};
