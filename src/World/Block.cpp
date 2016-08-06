#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "Utils/AABB.h"
#include "World/Block.h"
#include "World/World.h"

Block::Block(const sf::Vector3i& position, const sf::Color& color, const Type& type) : _position(position), _color(color), _type(type) {}

Block::Block() : Block(sf::Vector3i(0,0,0), sf::Color(0,0,0), Type(Type::AIR)) {}

sf::Vector3i Block::getPosition() const {
    return _position;
}

void Block::setPosition(const sf::Vector3i& position){
    _position = position;
}

sf::Color Block::getColor() const {
    return _color;
}

void Block::setColor(const sf::Color& color) {
    _color = color;
}

Block::Type Block::getType() const {
    return _type;
}

void Block::setType(const Block::Type& t) {
    _type = t;
}

AABB Block::getBoundingBox() const {
    if (_type == Type::AIR) {
        return AABB(sf::Vector3f(_position.x, _position.y, _position.z), sf::Vector3f(0, 0, 0));
    }
    return AABB(sf::Vector3f(_position.x, _position.y, _position.z), sf::Vector3f(1, 1, 1));
}
