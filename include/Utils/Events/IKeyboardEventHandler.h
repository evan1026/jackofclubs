#pragma once

#include <SFML/Window/Event.hpp>

/*!
 * Interface for classes that will handle keyboard events
 */
class IKeyboardEventHandler {
    public:

        /*! \callergraph
         *
         * Called when a keypress has occurred. Return true if event is finished being handled or false otherwise
         *
         * \p e - The event to handle
         */
        virtual bool handleKeyPressed(const sf::Event::KeyEvent& e) = 0;
};
