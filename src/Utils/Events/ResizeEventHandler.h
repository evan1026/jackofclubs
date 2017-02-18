#ifndef RESIZE_EVENT_HANDLER_H
#define RESIZE_EVENT_HANDLER_H

#include <SFML/Window/Event.hpp>

class ResizeEventHandler {
    public:
        virtual bool handleResize(const sf::Event::SizeEvent& e) = 0;
};

#endif
