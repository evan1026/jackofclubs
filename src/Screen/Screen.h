#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Window/Event.hpp>

class Screen {
    public:
        virtual void handleEvent(const sf::Event& event) = 0;
        virtual void tick() = 0;
};

#endif
