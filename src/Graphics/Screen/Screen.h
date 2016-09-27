#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Window/Event.hpp>

#include "Rendering/IRenderable.h"

/*
 * Base class for screens. Just defines some functions that need to
 * be overridden. (Not shown: IRenderable::render() also must be
 * overridden).
 */

class Screen : public IRenderable {
    public:
        virtual void handleEvent(const sf::Event& event) = 0;
        virtual void tick() = 0;
};

#endif
