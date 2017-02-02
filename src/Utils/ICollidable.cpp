#include "Utils/ICollidable.h"

/*! \callergraph
 *
 * Checks if two ICollidables are colliding
 *
 * \p other - The ICollidable to check for a collision with
 */
bool ICollidable::checkCollision(const ICollidable& other) const {
    return getBoundingBox().checkCollision(other.getBoundingBox());
}
