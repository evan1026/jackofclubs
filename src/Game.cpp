#include <SFML/Window.hpp>

#include "Debug/DebugOptions.h"
#include "Game.h"
#include "Graphics/Screen/WorldScreen.h"
#include "Logger/GlobalLogger.hpp"

using Logger::globalLogger;

/*
 * Constructs the game and initializes the render engine
 */
Game::Game() :
    _re()
{
    globalLogger.log("Initializing Game");
}

/*
 * Starts the game and runs the main loop, which polls events
 * and dispatches them to the appropriate handler.
 */
void Game::run() {
    globalLogger.log("Initializing World Screen");
    _screen = std::unique_ptr<Screen>(new WorldScreen(_re.getWindow(), *this));

    globalLogger.log("Getting Render Window");
    sf::RenderWindow& window = _re.getWindow();

    globalLogger.log("Starting main game loop");
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

/*
 * Ends the game by signalling the main loop should terminate
 */
void Game::end() {
    globalLogger.log("Stopping main game loop");
    _running = false;
}

/*
 * Handles a portion of the possible events.
 *
 * Currently handled events:
 *     1) Window Closed  - Ends the main loop
 *     2) Window Resized - Notifies the render engine
 *     3) F3 Pressed     - Marks debug options as available to be shown (screen handles actually showing them)
 *
 * Returns true if the event has been sufficiently handled (i.e. should not be forwarded along) and false otherwise
 */
bool Game::handleEvent(const sf::Event& e) {
    switch (e.type) {
        case sf::Event::Closed:
            end();
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
