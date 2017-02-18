#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SFML/Window/Event.hpp>

class EventHandler {
    public:
        virtual bool handleEvent(const sf::Event& e) = 0;
};

#endif
