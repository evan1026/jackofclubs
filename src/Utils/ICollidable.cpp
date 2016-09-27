#include "Utils/ICollidable.h"

/*
 * Checks if two ICollidables are colliding
 *
 * other - The ICollidable to check for a collision with
 */
bool ICollidable::checkCollision(const ICollidable& other) const {
    return getBoundingBox().checkCollision(other.getBoundingBox());
}
