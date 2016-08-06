#include <SFML/System.hpp>

#include "Utils/AABB.h"

AABB::AABB(const sf::Vector3f& position, const sf::Vector3f& size) : _position(position), _size(size) {}

sf::Vector3f AABB::getPosition() const {
    return _position;
}

void AABB::setPosition(const sf::Vector3f& pos) {
    _position = pos;
}

sf::Vector3f AABB::getSize() const {
    return _size;
}

void AABB::setSize(const sf::Vector3f& size) {
    _size = size;
}

sf::Vector3f AABB::getCenter() const {
    return _position + (_size / 2.f);
}

bool AABB::checkCollision(const AABB& other) const {
    //Base case, size == 0 for all dimensions, denoting object can be passed through
    if ((_size.x == 0 && _size.y == 0 && _size.z == 0)
            || (other._size.x == 0 && other._size.y == 0 && other._size.z == 0)) {
        return false;
    }

    //Collision exists if all dimensions have overlap

    bool collision = true;

    collision &= checkOverlap(_position.x, _position.x + _size.x, other._position.x, other._position.x + other._size.x);
    collision &= checkOverlap(_position.y, _position.y + _size.y, other._position.y, other._position.y + other._size.y);
    collision &= checkOverlap(_position.z, _position.z + _size.z, other._position.z, other._position.z + other._size.z);

    return collision;
}

bool AABB::checkOverlap(float low1, float high1, float low2, float high2) const {
    return (!(high1 <= low2 || low1 >= high2));
}
