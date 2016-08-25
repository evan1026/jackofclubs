#include <SFML/System.hpp>

#include "Debug/DebugOptions.h"
#include "Player.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
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
        e.renderAABB(getBoundingBox(), sf::Color::White);
}
