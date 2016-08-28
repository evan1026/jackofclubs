#include <SFML/Window.hpp>

#include "Debug/DebugOptions.h"
#include "Game.h"
#include "Graphics/Screen/WorldScreen.h"

Game::Game() :
    _re(1920, 1080) //TODO detect screen size or select a default
    {}

void Game::run() {
    _screen = std::unique_ptr<Screen>(new WorldScreen(_re.getWindow(), *this));
    sf::RenderWindow& window = _re.getWindow();

    while (_running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (!handleEvent(event)) {
                _screen->handleEvent(event);
            }
        }
        _screen->tick();
        _re.beginRender();
        _screen->render(_re, window);
        _re.endRender();
    }
}

void Game::end() {
    _running = false;
}

bool Game::handleEvent(const sf::Event& e) {
    switch (e.type) {
        case sf::Event::Closed:
            _running = false;
            return true;
        case sf::Event::Resized:
            _re.handleResize(e.size.width, e.size.height);
            return false;
        case sf::Event::KeyPressed:
            if (e.key.code == sf::Keyboard::F3) {
                DebugOptions::setShowDebugOptions(!DebugOptions::showDebugOptions());
                return true;
            }
            return false;
        default:
            return false;
    }
}
