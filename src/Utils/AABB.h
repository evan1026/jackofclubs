#ifndef AABB_H
#define AABB_H

#include <SFML/System.hpp>

class AABB {

    sf::Vector3f _position;
    sf::Vector3f _size;

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
