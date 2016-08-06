#include <SFML/System.hpp>
#include <vector>

#include "Player.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
#include "World/Block.h"
#include "World/World.h"

Player::Player(const Type& type, const sf::Vector3f& position, const sf::Vector3f& rotation)
    : _type(type), _position(position), _rotation(rotation) {}

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
    } else if (Math::signum(startVel) > 0) {
        return startVel + (std::ceil(endPos) - endPos);
    } else {
        return 0;
    }
}

sf::Vector3f Player::move(const sf::Vector3f& velocity) {
    std::vector<Block> collidingBlocks;
    sf::Vector3f startPos = _position;
    sf::Vector3f ourCenter;
    sf::Vector3f finalVelocity = velocity;
    sf::Vector3f collisionSum;

    _position += velocity;
    ourCenter = getBoundingBox().getCenter();

    collidingBlocks = World::getInst().checkCollision(*this);

    for (Block block : collidingBlocks) {
        sf::Vector3f theirCenter = block.getBoundingBox().getCenter();
        sf::Vector3f collision = ourCenter - theirCenter;

        collision.x = Math::signum(collision.x);
        collision.y = Math::signum(collision.y);
        collision.z = Math::signum(collision.z);

        collisionSum += collision;
    }

    if (Math::signum(collisionSum.x) == -Math::signum(velocity.x)) {
        finalVelocity.x = shrinkVelocity(velocity.x, _position.x);
    }
    if (Math::signum(collisionSum.y) == -Math::signum(velocity.y)) {
        finalVelocity.y = shrinkVelocity(velocity.y, _position.y);
    }
    if (Math::signum(collisionSum.z) == -Math::signum(velocity.z)) {
        finalVelocity.z = shrinkVelocity(velocity.z, _position.z);
    }

    _position = startPos + finalVelocity;

    return finalVelocity;
}
