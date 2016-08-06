#include "Utils/ICollidable.h"

bool ICollidable::checkCollision(const ICollidable& other) const {
    return getBoundingBox().checkCollision(other.getBoundingBox());
}
