#include <SFML/Window.hpp>

#include <GlobalLogger.hpp>

#include "Debug/DebugOptions.h"
#include "Game.h"
#include "Graphics/Screen/WorldScreen.h"

using Logger::globalLogger;

/*! \callergraph
 *
 * Constructs the game and initializes the render engine
 */
Game::Game() :
    _re()
{
    globalLogger.log("Initializing Game");
}

/*! \callergraph
 *
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

/*! \callergraph
 *
 * Ends the game by signalling the main loop should terminate
 */
void Game::end() {
    globalLogger.log("Stopping main game loop");
    _running = false;
}

/*! \callergraph
 *
 * Handles a portion of the possible events.
 *
 * Currently handled events:                                                                                      <br>
 *     1) Window Closed  - Ends the main loop                                                                     <br>
 *     2) Window Resized - Notifies the render engine                                                             <br>
 *     3) F3 Pressed     - Marks debug options as available to be shown (screen handles actually showing them)    <br>
 *
 * Returns true if the event has been sufficiently handled (i.e. should not be forwarded along) and false otherwise
 *
 * \p e - The event to process
 */
bool Game::handleEvent(const sf::Event& e) {
    switch (e.type) {
        case sf::Event::Closed:
            end();
            return true;
        case sf::Event::Resized:
            _re.handleResize(e.size);
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
