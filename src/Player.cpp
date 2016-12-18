#include <SFML/System.hpp>

#include "Debug/DebugOptions.h"
#include "Player.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
#include "Utils/Maybe.h"
#include "World/BlockFace.h"
#include "World/World.h"

#define AABB_INSET 0.25
#define AABB_INSET_SCALED AABB_INSET * 99 / 100 // Needed because floating point is inexact, and when we
                                                // use the "exact" value to adust the velocity, it rounds
                                                // to put us inside of a block

/*
 * type     - The Player::Type of this player (either SELF or OTHER)
 * position - The player's initial position
 * rotation - The player's initial look direction
 */
Player::Player(const Type& type, const sf::Vector3f& position, const sf::Vector3f& rotation)
        : _type(type), _position(position), _rotation(rotation)
{}

/*
 * Returns the AABB (axis-aligned bounding box) that surrounds the player
 * Player is almost 1x2x1 (actually 0.9x1.9x0.9)
 */
AABB Player::getBoundingBox() const {
    return AABB(_position, sf::Vector3f(1 - AABB_INSET, 2, 1 - AABB_INSET));
}

/*
 * Returns the player's current position
 */
sf::Vector3f Player::getPosition() const {
    return _position;
}

/*
 * Returns the player's current look direction
 */
sf::Vector3f Player::getRotation() const {
    return _rotation;
}

/*
 * Returns the player's current velocity
 */
sf::Vector3f Player::getVelocity() const {
    return _velocity;
}

/*
 * Sets the player's rotation (look direction)
 *
 * rotation - The new rotation
 */
void Player::setRotation(const sf::Vector3f& rotation) {
    _rotation = rotation;
}

/*
 * Sets player's position directly
 * WARNING: Using this instead of the normal velocity system could lead
 *          to the player getting stuck in a block
 *
 * position - The new position
 */
void Player::setPosition(const sf::Vector3f& position) {
    _position = position;
}

/*
 * Sets the velocity of the player, which then gets applied when Player::tick
 * is called
 *
 * velocity - The new velocity
 */
void Player::setVelocity(const sf::Vector3f& velocity) {
    _velocity = velocity;
}

/*
 * Used by move() to shrink the player's velocity along one axis
 * when they're about to collide with a block so that they end
 * up right on the edge instead.
 *
 * startVel - The initial velocity they're trying to move with
 * endPos   - Where they would end up if they moved with that velocity
 */
float Player::shrinkVelocity(const float startVel, const float endPos, const float inset) const {
    if (Math::signum(startVel) > 0) {
        return startVel - (endPos - std::floor(endPos)) + inset;
    } else if (Math::signum(startVel) < 0) {
        return startVel + (std::ceil(endPos) - endPos); // No adjustment, since _position already represents the lower edge of our hitbox
    } else {
        return 0;
    }
}

/*
 * Applies the player's velocity to their position, also keeping them from colliding with a
 * block if that velocity would cause a collision.
 *
 * Works by splitting the velocity into its seperate components (x,y,z) and then applying
 * them in order. If the application of one component causes the player to collide with a block,
 * the velocity is adjusted so they are put right up against the nearest block boundary in
 * that direction.
 *
 * The order that they directions is applied is as follows:
 *     y is applied first, to make sure gravity (once implemented) will always be applied
 *     x is applied second, because something needs to go next, and x was selected arbitrarily
 *     z is applied last, because it's the one left
 *
 * WARNING: This assumes the player's velocity will always be < 1. If this ever does not hold,
 * either this or Player::shrinkVelocity (or both) will need to be adjusted.
 *
 * velocity - Essentially the distance they should move in this tick
 * world    - The world they're moving in (used to check collisions)
 */
void Player::tick(const World& world) {
    sf::Vector3f startPos = _position;
    sf::Vector3f finalVelocity = _velocity;

    _position.y += _velocity.y;
    if (world.checkCollision(*this)) {
        finalVelocity.y = shrinkVelocity(_velocity.y, _position.y, 0);
        _position.y = startPos.y + finalVelocity.y;
    }

    _position.x += _velocity.x;
    if (world.checkCollision(*this)) {
        finalVelocity.x = shrinkVelocity(_velocity.x, _position.x, AABB_INSET_SCALED);
        _position.x = startPos.x + finalVelocity.x;
    }

    _position.z += _velocity.z;
    if (world.checkCollision(*this)) {
        finalVelocity.z = shrinkVelocity(_velocity.z, _position.z, AABB_INSET_SCALED);
        _position.z = startPos.z + finalVelocity.z;
    }

    _velocity = finalVelocity;
}

/*
 * Handles applying camera rotation and translation, as well as
 * rendering the hitbox if that debug option is enabled.
 *
 * e - The rendering engine (used for OpenGL calls, like rotate and translate)
 * w - The window (used for SFML calls; unused here)
 */
void Player::render(RenderEngine& e, sf::RenderWindow& w) {
    if (_type == Type::SELF) {
        e.rotatePlayer(*this);
        e.translatePlayer(*this);
    }
    // TODO else render other player

    if (DebugOptions::playerHitboxRendered())
        e.renderAABB(getBoundingBox(), sf::Color::Black);
}

/*
 * Used by getSelection() to find the smallest initial t value to get us to a block border
 * in a certain direction.
 *
 * See getSelection() for more.
 *
 * origin    - Start position
 * direction - How much we're moving in this direction
 */
float Player::getTMax(float origin, float direction) const {
    if (direction > 0) {
        return (Math::ceil(origin) - origin) / Math::abs(direction);
    } else {
        return (origin - Math::floor(origin)) / Math::abs(direction);
    }
}

/*
 * Implementation of the algorithm described at http://www.cse.chalmers.se/edu/year/2011/course/TDA361/grid.pdf
 *
 * Uses ray tracing to find the first block directly in front of the player. Uses the
 * idea that any position on the ray can be modeled by u = td + o, where d is the direction the ray goes,
 * o is the origin of the ray, and t is some number. The algorithm calculates every t value needed to get
 * from the block it's currently in to the next block the ray intersects. It keeps following the ray
 * until it finds a block that is solid, or until it goes out of range. See the paper for more information.
 *
 * world - The world to trace through
 * range - The distance (in blocks) when we should say the selection (if there is one) is out of range
 */
Maybe<BlockFace> Player::getSelection(World& world, float range) const {
    sf::Vector3f direction = sf::Vector3f(Math::sinDeg(_rotation.y) * Math::cosDeg(_rotation.x), -Math::sinDeg(_rotation.x), -Math::cosDeg(_rotation.y) * Math::cosDeg(_rotation.x));
    sf::Vector3f size = getBoundingBox().getSize();
    sf::Vector3f origin = _position + sf::Vector3f(size.x / 2, size.y * 3 / 4, size.z / 2);
    sf::Vector3f pos(Math::floor(origin.x), Math::floor(origin.y), Math::floor(origin.z));
    sf::Vector3f step(Math::signum(direction.x), Math::signum(direction.y), Math::signum(direction.z));
    sf::Vector3f tMax(getTMax(origin.x, direction.x), getTMax(origin.y, direction.y), getTMax(origin.z, direction.z));
    sf::Vector3f tDelta(step.x / direction.x, step.y / direction.y, step.z / direction.z);
    sf::Vector3f normal;

    // Puts the range in terms of t
    range /= Math::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

    // Loops until it finds a solid block, or until it's out of range
    while (world.getBlockType(sf::Vector3i(pos.x, pos.y, pos.z)) != Block::Type::SOLID) {

        // All of this just finds the next t we need to consider
        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                if (tMax.x > range) break;
                pos.x += step.x;
                tMax.x += tDelta.x;
                normal = sf::Vector3f(-step.x, 0, 0);
            } else {
                if (tMax.z > range) break;
                pos.z += step.z;
                tMax.z += tDelta.z;
                normal = sf::Vector3f(0, 0, -step.z);
            }
        } else {
            if (tMax.y < tMax.z) {
                if (tMax.y > range) break;
                pos.y += step.y;
                tMax.y += tDelta.y;
                normal = sf::Vector3f(0, -step.y, 0);
            } else {
                if (tMax.z > range) break;
                pos.z += step.z;
                tMax.z += tDelta.z;
                normal = sf::Vector3f(0, 0, -step.z);
            }
        }
    }

    // If the last position we considered is actually a solid block, we have a valid selection,
    // so return it, and return nothing otherwise
    sf::Vector3i finalPosition(pos.x, pos.y, pos.z);
    if (world.getBlockType(finalPosition) == Block::Type::SOLID) {
        return Maybe<BlockFace>(pos, normal);
    } else {
        return Maybe<BlockFace>();
    }
}
