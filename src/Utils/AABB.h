#ifndef AABB_H
#define AABB_H

#include <SFML/System.hpp>

/*
 * Axis aligned bounding box
 * Basically just a box surrounding some thing with
 * sides that are aligned to x,y,z
 *
 * Used for easy collision checking
 */

class AABB {

    sf::Vector3f _position;
    sf::Vector3f _size;

    // Checks if there is an overlap in one dimension
    bool checkOverlap(float low1, float high1, float low2, float high2) const;

    public:
        AABB(const sf::Vector3f& position, const sf::Vector3f& size);

        sf::Vector3f getPosition() const;
        void setPosition(const sf::Vector3f&);

        sf::Vector3f getSize() const;
        void setSize(const sf::Vector3f&);

        sf::Vector3f getCenter() const;

        bool checkCollision(const AABB& other) const;
};

#endif
