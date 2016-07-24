#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "IRenderable.h"

class Block : public IRenderable {
    sf::Vector3f _position;
    sf::Color _color;

    public:
        static const float model[6][12];

        Block(const sf::Vector3f& position, const sf::Color& color) : _position(position), _color(color) {
            addToEngine();
        }

        ~Block() {
            removeFromEngine();
        }

        sf::Vector3f getPosition() const;
        void setPosition(const sf::Vector3f& position);

        sf::Color getColor() const;
        void setColor(const sf::Color& color);

        void render(RenderEngine&) const;
};

#endif
