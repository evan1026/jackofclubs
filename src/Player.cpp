#include <SFML/System.hpp>

#include "Debug/DebugOptions.h"
#include "Player.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
#include "Utils/Maybe.h"
#include "World/BlockFace.h"
#include "World/World.h"

Player::Player(const Type& type, const sf::Vector3f& position, const sf::Vector3f& rotation)
        : _type(type), _position(position), _rotation(rotation)
{}

AABB Player::getBoundingBox() const {
    if (_type == Type::SELF) { //Done to silence warnings. TODO remove later
        AABB(_position, sf::Vector3f(1, 2, 1));
    }
    return AABB(_position, sf::Vector3f(1, 2, 1));
}

const sf::Vector3f& Player::getPosition() const {
    return _position;
}

const sf::Vector3f& Player::getRotation() const {
    return _rotation;
}

void Player::setRotation(const sf::Vector3f& rotation) {
    _rotation = rotation;
}

float Player::shrinkVelocity(const float startVel, const float endPos) const {
    if (Math::signum(startVel) > 0) {
        return startVel - (endPos - std::floor(endPos));
    } else if (Math::signum(startVel) < 0) {
        return startVel + (std::ceil(endPos) - endPos);
    } else {
        return 0;
    }
}

void Player::move(const sf::Vector3f& velocity, const World& world) {
    sf::Vector3f startPos = _position;
    sf::Vector3f finalVelocity = velocity;

    // Algorithm is fairly simple. Just try each direction and see if there's a collision.
    // If there is, shrink the velocity to put us right up against a block.
    // The reason for doing this instead of doing them all at once is that it can cause
    // collsions in one direction that cancel out velocity in a direction that is perfectly
    // fine. For instance, if you have gravity pulling you down and you try to move forward,
    // the forward movement will be cancelled due to collision with the ground
    // The order in which these are done is to have y done first so that movement doesn't
    // interfere with gravity, and then the rest of the order is arbitrary

    _position.y += velocity.y;
    if (world.checkCollision(*this)) {
        finalVelocity.y = shrinkVelocity(velocity.y, _position.y);
        _position.y = startPos.y + finalVelocity.y;
    }

    _position.x += velocity.x;
    if (world.checkCollision(*this)) {
        finalVelocity.x = shrinkVelocity(velocity.x, _position.x);
        _position.x = startPos.x + finalVelocity.x;
    }

    _position.z += velocity.z;
    if (world.checkCollision(*this)) {
        finalVelocity.z = shrinkVelocity(velocity.z, _position.z);
        _position.z = startPos.z + finalVelocity.z;
    }
}

void Player::render(RenderEngine& e, sf::RenderWindow& w) {
    e.rotatePlayer(_rotation);
    e.translatePlayer(_position);

    if (DebugOptions::playerHitboxRendered())
        e.renderAABB(getBoundingBox(), sf::Color::Black);
}

float Player::getTMax(float origin, float direction) const {
    if (direction > 0) {
        return (Math::ceil(origin) - origin) / Math::abs(direction);
    } else {
        return (origin - Math::floor(origin)) / Math::abs(direction);
    }
}

// Implementation of the algorithm described at http://www.cse.chalmers.se/edu/year/2011/course/TDA361/grid.pdf
Maybe<BlockFace> Player::getSelection(World& world, float range) const {
    sf::Vector3f direction = sf::Vector3f(Math::sinDeg(_rotation.y) * Math::cosDeg(_rotation.x), -Math::sinDeg(_rotation.x), -Math::cosDeg(_rotation.y) * Math::cosDeg(_rotation.x));
    sf::Vector3f origin = _position + sf::Vector3f(0.5, 1.75, 0.5);
    sf::Vector3f pos(Math::floor(origin.x), Math::floor(origin.y), Math::floor(origin.z));
    sf::Vector3f step(Math::signum(direction.x), Math::signum(direction.y), Math::signum(direction.z));
    sf::Vector3f tMax(getTMax(origin.x, direction.x), getTMax(origin.y, direction.y), getTMax(origin.z, direction.z));
    sf::Vector3f tDelta(step.x / direction.x, step.y / direction.y, step.z / direction.z);
    sf::Vector3f normal;

    range /= Math::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

    while (world.getBlockType(sf::Vector3i(pos.x, pos.y, pos.z)) != Block::Type::SOLID) {
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

    sf::Vector3i finalPosition(pos.x, pos.y, pos.z);
    if (world.getBlockType(finalPosition) == Block::Type::SOLID) {
        return Maybe<BlockFace>(BlockFace(pos, normal));
    } else {
        return Maybe<BlockFace>();
    }
}
