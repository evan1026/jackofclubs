#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

class Block {
    public:
        enum class Type {
            AIR,
            SOLID
        };

    private:
        sf::Vector3i _position;
        sf::Color _color;
        Type _type;

    public:
        static constexpr float SIZE = 30;

        Block(const sf::Vector3i& position, const sf::Color& color, const Type& type)
            : _position(position), _color(color), _type(type) {}

        Block() : Block(sf::Vector3i(0,0,0), sf::Color(0,0,0), Type(Type::AIR)) {}

        sf::Vector3i getPosition() const;
        void setPosition(const sf::Vector3i& position);

        sf::Color getColor() const;
        void setColor(const sf::Color& color);

        Type getType() const;
        void setType(const Type& t);
};

#endif
