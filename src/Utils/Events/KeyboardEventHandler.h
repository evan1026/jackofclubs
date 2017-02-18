#ifndef KEYBOARD_EVENT_HANDLER_H
#define KEYBOARD_EVENT_HANDLER_H

#include <SFML/Window/Event.hpp>

class KeyboardEventHandler {
    public:
        virtual bool handleKeyPressed(const sf::Event::KeyEvent& e) = 0;
};

#endif
