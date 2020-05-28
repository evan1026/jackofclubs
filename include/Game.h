#ifndef GAME_H
#define GAME_H

#include <memory>

#include "Graphics/Screen/Screen.h"
#include "Rendering/RenderEngine.h"

/*!
 * Represents the game as a while.
 * Mostly just handles dispatching events and invoking rendering.
 */
class Game {

    std::unique_ptr<Screen> _screen;
    bool _running = true;
    RenderEngine _re;

    //Returns true if event is done being handled
    bool handleEvent(const sf::Event& e);

    public:
        void run();
        void end();

        Game();
};

#endif
