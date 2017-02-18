#ifndef MOUSE_EVENT_HANDLER_H
#define MOUSE_EVENT_HANDLER_H

#include <SFML/Window/Event.hpp>

class MouseEventHandler {
    public:
        virtual bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) = 0;
        virtual bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) = 0;
        virtual bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) = 0;
};

#endif
