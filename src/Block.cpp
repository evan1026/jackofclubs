#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "Block.h"
#include "RenderEngine.h"
#include "World.h"

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
