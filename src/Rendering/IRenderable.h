#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <SFML/Graphics.hpp>

#include "RenderEngine.h"

/*
 * Defines a rendering function so there is a common interface for rendered objects.
 */

class IRenderable {
    public:
        virtual void render(RenderEngine&, sf::RenderWindow&) = 0;
};

#endif
