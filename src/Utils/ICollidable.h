#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H

#include "Utils/AABB.h"

/*!
 * Interface for collidable objects.
 *
 * Simply forces them to provide a function to get their AABB
 * and provides a function to check if there is a collision.
 */
class ICollidable {
    public:
        /*! Abstract function forcing child classes to provide their bounding box */
        virtual AABB getBoundingBox() const = 0;

        bool checkCollision(const ICollidable& other) const;
};

#endif
