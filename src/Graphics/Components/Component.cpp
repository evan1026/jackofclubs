#include "Graphics/Components/Component.h"

Component::Component(const sf::Vector2i& localPos, const sf::Vector2i& parentPos, const sf::Vector2i& size) :
    _boundingBox(parentPos + localPos, size),
    _localPos(localPos)
{}

void Component::setSize(const sf::Vector2i& size) {
    _boundingBox.width = size.x;
    _boundingBox.height = size.y;
}

void Component::setGlobalPosition(const sf::Vector2i& pos) {
    _boundingBox.left = pos.x;
    _boundingBox.top = pos.y;
}

void Component::setLocalPosition(const sf::Vector2i& pos) {
    sf::Vector2i parentPos = getParentPosition();
    _localPos = pos;
    setGlobalPosition(parentPos + _localPos);
}

void Component::setParentPosition(const sf::Vector2i& pos) {
    setGlobalPosition(pos + _localPos);
}

sf::Vector2i Component::getSize() {
    return sf::Vector2i(_boundingBox.width, _boundingBox.height);
}

sf::Vector2i Component::getLocalPosition() {
    return _localPos;
}

sf::Vector2i Component::getGlobalPosition() {
    return sf::Vector2i(_boundingBox.left, _boundingBox.top);
}

sf::Vector2i Component::getParentPosition() {
    return getGlobalPosition() - getLocalPosition();
}
