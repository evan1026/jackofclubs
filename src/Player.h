#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System.hpp>

#include "Rendering/IRenderable.h"
#include "Utils/AABB.h"
#include "Utils/ICollidable.h"

class Player : public ICollidable, public IRenderable {
    public:
        enum class Type {
            SELF,
            OTHER
        };

    private:
        Type _type;
        sf::Vector3f _position;
        sf::Vector3f _rotation;
        bool _rendered;

        float shrinkVelocity(const float startVel, const float endPos) const;

    public:
        Player(const Type& type, const sf::Vector3f& position, const sf::Vector3f& rotation);
        AABB getBoundingBox() const;

        const sf::Vector3f& getPosition() const;
        const sf::Vector3f& getRotation() const;

        void setRendered(const bool render);
        bool getRendered() const;
        void render(RenderEngine& e);

        void setRotation(const sf::Vector3f& rotation);
        sf::Vector3f move(const sf::Vector3f& velocity);
};

#endif
