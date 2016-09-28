#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System.hpp>

#include "Rendering/IRenderable.h"
#include "Rendering/RenderEngine.h"
#include "Utils/AABB.h"
#include "Utils/ICollidable.h"
#include "Utils/Maybe.h"
#include "World/BlockFace.h"

class World;

/*
 * Keeps information about the player like their position
 * and which way they're looking and contains functionality
 * to make sure they don't collide with a block when moving.
 * Also handles finding the block selection.
 */

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

        // Used by move() to put you right up against a block you're about to collide with
        float shrinkVelocity(const float startVel, const float endPos) const;

        // Used by getSelection() to get the first block we'll collide with, see getSelection() for more
        float getTMax(float origin, float direction) const;

    public:
        Player(const Type& type, const sf::Vector3f& position, const sf::Vector3f& rotation);

        AABB getBoundingBox() const override;

        sf::Vector3f getPosition() const;
        sf::Vector3f getRotation() const;

        void render(RenderEngine& e, sf::RenderWindow& w) override;

        void setRotation(const sf::Vector3f& rotation);
        void move(const sf::Vector3f& velocity, const World& world); // move rather than setPosition so we can do collision checking

        Maybe<BlockFace> getSelection(World& world, float range) const;
};

#endif
