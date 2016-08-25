#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <SFML/Graphics.hpp>

class RenderEngine;

// Interface for renderable objects. Used by RenderEngine.
class IRenderable {
    public:
        virtual void render(RenderEngine&, sf::RenderWindow&) = 0;
};

#endif
