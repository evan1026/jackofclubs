#pragma once

#include <SFML/System.hpp>

#include "Rendering/Renderable.h"
#include "Rendering/RenderEngine.h"
#include "Utils/AABB.h"
#include "Utils/ICollidable.h"
#include "Utils/Maybe.h"
#include "World/BlockFace.h"
#include "Rendering/BlockSelection.h"

class World;

/*!
 * Keeps information about the player like their position
 * and which way they're looking and contains functionality
 * to make sure they don't collide with a block when moving.
 * Also handles finding the block selection.
 */
class Player : public ICollidable, public Renderable {
    public:
        /*! Quick enumeration to distinguish between oneself and others */
        enum class Type {
            SELF,
            OTHER
        };

        Maybe<BlockFace> getSelection(const World& world, float range) const;

    private:
        Type _type;
        sf::Vector3f _position;
        sf::Vector3f _rotation;
        sf::Vector3f _velocity;
        bool _jumping;
        BlockSelection _selection;

        // Used by move() to put you right up against a block you're about to collide with
        float shrinkVelocity(const float startVel, const float endPos, const float inset) const;

        // Used by getSelection() to get the first block we'll collide with, see getSelection() for more
        float getTMax(float origin, float direction) const;

    public:
        Player(const Type& type, const sf::Vector3f& position, const sf::Vector3f& rotation);

        AABB getBoundingBox() const override;

        sf::Vector3f getPosition() const;
        sf::Vector3f getRotation() const;
        sf::Vector3f getVelocity() const;

        void render(RenderEngine& e, sf::RenderWindow& w) override;

        void setRotation(const sf::Vector3f& rotation);
        void setVelocity(const sf::Vector3f& velocity);
        void setPosition(const sf::Vector3f& position);

        void tick(const World& world);

        inline std::optional<BlockFace> getSelection() const { return _selection.getSelection(); }

        void setJumping(bool jumping);
        bool getJumping() const;;

        sf::Vector3f getHeadLocation() const;
};
