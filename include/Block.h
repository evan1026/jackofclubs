#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "IRenderable.h"
#include "RenderEngine.h"

class Block : public IRenderable {
    sf::Vector3i _position;
    sf::Color _color;

    public:
        static const float model[6][12];
        static constexpr float UNIT_LENGTH = 15;
        static constexpr float SIZE = UNIT_LENGTH * 2;

        Block(const sf::Vector3i& position, const sf::Color& color) : _position(position), _color(color) {}

        Block() : Block(sf::Vector3i(0,0,0), sf::Color(0,0,0)) {}

        sf::Vector3i getPosition() const;
        void setPosition(const sf::Vector3i& position);

        sf::Color getColor() const;
        void setColor(const sf::Color& color);

        void render(RenderEngine&) const;
};

#endif
