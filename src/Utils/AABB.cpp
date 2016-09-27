#include <SFML/System.hpp>

#include "Utils/AABB.h"

/*
 * position - bottom left back corner of the box
 * size     - width, length, and height of the box
 */
AABB::AABB(const sf::Vector3f& position, const sf::Vector3f& size) : _position(position), _size(size) {}

/*
 * Returns the bottom left back corner
 */
sf::Vector3f AABB::getPosition() const {
    return _position;
}

/*
 * pos - new bottom left back corner
 */
void AABB::setPosition(const sf::Vector3f& pos) {
    _position = pos;
}

/*
 * Returns the dimensions of the box
 */
sf::Vector3f AABB::getSize() const {
    return _size;
}

/*
 * size - new dimensions of the box
 */
void AABB::setSize(const sf::Vector3f& size) {
    _size = size;
}

/*
 * Returns the point at the exact center of the box
 */
sf::Vector3f AABB::getCenter() const {
    return _position + (_size / 2.f);
}

/*
 * Returns true if we are colliding with the other box
 *
 * other - The AABB to check if we are colliding with
 */
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

/*
 * Checks if there is overlap in one dimension
 *
 * For a1,a2,b1,b2, there is only not overlap in these two cases:
 *
 *     |----|    |----|            |----|    |----|
 *     a1   a2   b1   b2           b1   b2   a1   a2
 *
 * So what we do is check if it's one of those cases and then return the opposite of that.
 *
 * low1  - object 1's low value  (a1)
 * high1 - object 1's high value (a2)
 * low2  - object 2's low value  (b1)
 * high2 - object 2's high value (b2)
 */
bool AABB::checkOverlap(float low1, float high1, float low2, float high2) const {
    return (!(high1 <= low2 || low1 >= high2));
}
