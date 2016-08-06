#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H

#include "Utils/AABB.h"

class ICollidable {
    public:
        virtual AABB getBoundingBox() const = 0; //Return multiple just in case of complex shapes
        bool checkCollision(const ICollidable& other) const;
};

#endif
