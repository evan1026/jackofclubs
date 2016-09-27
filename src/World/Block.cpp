#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "Utils/AABB.h"
#include "World/Block.h"
#include "World/World.h"

/*
 * Constructs a block
 *
 * position - bottom left back corner
 * color    - block color
 * type     - Either Block::Type::SOLID or Block::Type::AIR
 */
Block::Block(const sf::Vector3i& position, const sf::Color& color, const Type& type) : _position(position), _color(color), _type(type) {}

/*
 * Default constructor just because std::array needs default constructable objects
 */
Block::Block() : Block(sf::Vector3i(0,0,0), sf::Color(0,0,0), Type(Type::AIR)) {}

/*
 * Returns the bottom left back corner
 */
sf::Vector3i Block::getPosition() const {
    return _position;
}

/*
 * position - new bottom left back corner
 */
void Block::setPosition(const sf::Vector3i& position){
    _position = position;
}

/*
 * Returns the block's color
 */
sf::Color Block::getColor() const {
    return _color;
}

/*
 * color - new block color
 */
void Block::setColor(const sf::Color& color) {
    _color = color;
}

/*
 * Returns the block's Block::Type
 */
Block::Type Block::getType() const {
    return _type;
}

/*
 * t - new Block::Type
 */
void Block::setType(const Block::Type& t) {
    _type = t;
}

/*
 * Returns AABB with size (0,0,0) if air (indicates that it can be passed through),
 * or one with size (1,1,1) otherwise
 */
AABB Block::getBoundingBox() const {
    if (_type == Type::AIR) {
        return AABB(sf::Vector3f(_position.x, _position.y, _position.z), sf::Vector3f(0, 0, 0));
    }
    return AABB(sf::Vector3f(_position.x, _position.y, _position.z), sf::Vector3f(1, 1, 1));
}
