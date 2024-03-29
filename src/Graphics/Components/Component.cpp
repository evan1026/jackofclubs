#include <algorithm>
#include <stdexcept>

#include "Graphics/Components/Component.h"

/*! \callergraph
 *
 * Constucts a new component based on 
 * its position relative to the parent.
 *
 * \p localPos  - The position of the component relative to its parent                              <br>
 * \p size      - The length and width of the rectangular bounding box surrounding the component    <br>
 */
Component::Component(const sf::Vector2i& localPos, const sf::Vector2i& size, bool children) :
    _boundingBox(localPos, size),
    _localPos(localPos),
    _childrenAllowed(children)
{}

Component::Component(const sf::Vector2i& size, bool children) :
    _boundingBox(sf::Vector2i(0,0), size),
    _localPos(sf::Vector2i(0,0)),
    _childrenAllowed(children)
{}

Component::Component(bool children) : Component(sf::Vector2i(0,0)) {}

/*! \callergraph
 *
 * Sets the size of the component
 *
 * \p size - new size
 */
void Component::setSize(const sf::Vector2i& size) {
    _boundingBox.width = size.x;
    _boundingBox.height = size.y;
}

void Component::setSize(int x, int y) {
    setSize(sf::Vector2i(x, y));
}

/*! \callergraph
 *
 * Sets the global position of the component. This is the
 * position that's sent to the window for rendering. This
 * function is private and is called from the other
 * set*Position functions.
 *
 * \p pos - global position
 */
void Component::setGlobalPosition(const sf::Vector2i& pos) {
    _boundingBox.left = pos.x;
    _boundingBox.top = pos.y;
}

/*! \callergraph
 *
 * Sets the component's position relative to its parent
 *
 * \p pos - position relative to parent
 */
void Component::setLocalPosition(const sf::Vector2i& pos) {
    sf::Vector2i parentPos = getParentPosition();
    _localPos = pos;
    setGlobalPosition(parentPos + _localPos);
}

void Component::setLocalPosition(int x, int y) {
    setLocalPosition(sf::Vector2i(x, y));
}

/*! \callergraph
 *
 * Notifies the component of its parent's position so it
 * can recalculate its global position.
 *
 * \p pos - parent's global position
 */
void Component::setParentPosition(const sf::Vector2i& pos) {
    setGlobalPosition(pos + _localPos);
}

/*! \callergraph
 *
 * Returns the length and width of the rectangular bounding box
 * surrounding the component
 */
const sf::Vector2i Component::getSize() const {
    return sf::Vector2i(_boundingBox.width, _boundingBox.height);
}

/*! \callergraph
 *
 * Returns the component's position relative to its parent
 */
const sf::Vector2i Component::getLocalPosition() const {
    return _localPos;
}

/*! \callergraph
 *
 * Returns the component's global position. Note that, unlike
 * setGlobalPosition(), this function is public
 */
const sf::Vector2i Component::getGlobalPosition() const {
    return sf::Vector2i(_boundingBox.left, _boundingBox.top);
}

/*! \callergraph
 *
 * Returns what the component thinks its parent's position is.
 * This may or may not be the parent's actual position.
 */
const sf::Vector2i Component::getParentPosition() const {
    return getGlobalPosition() - getLocalPosition();
}

/*! \callergraph
 *
 * Returns the bounding box of the component
 */
const sf::Rect<int> Component::getBounds() const {
    return _boundingBox;
}

void Component::forceAdd(std::shared_ptr<Component> component) {
    _children.push_back(component);
}

std::shared_ptr<Component> Component::forceRemove(std::shared_ptr<Component> component) {
    auto pos = std::find(_children.begin(), _children.end(), component);
    if (pos != _children.end()) {
        _children.erase(pos);
        return component;
    }
    return nullptr;
}

void Component::add(std::shared_ptr<Component> component) {
    if (_childrenAllowed) {
        forceAdd(component);
    } else {
        throw std::invalid_argument("Trying to add a child component when doing so is disallowed");
    }
}

std::shared_ptr<Component> Component::remove(std::shared_ptr<Component> component) {
    if (_childrenAllowed) {
        return forceRemove(component);
    } else {
        throw std::invalid_argument("Trying to remove a child component when doing so is disallowed");
    }
}

void Component::render(RenderEngine& e, sf::RenderWindow& w) {
    renderComponent(w);
    for (std::shared_ptr<Component> comp : _children) {
        comp->setParentPosition(getGlobalPosition());
        comp->render(e, w);
    }
}

void Component::renderComponent(sf::RenderWindow& w) {}

void Component::layout(const sf::RenderWindow& w) {
    for (auto child : _children) {
        child->layout(w);
    }
}

bool Component::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return delegateToChildren([e](auto& child){return child.handleKeyPressed(e);});
}

bool Component::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    return delegateToChildren([e](auto& child){return child.handleMouseMoved(e);});
}

bool Component::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    return delegateToChildren([e](auto& child){return child.handleMouseButtonPressed(e);});
}

bool Component::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return delegateToChildren([e](auto& child){return child.handleMouseButtonReleased(e);});
}

bool Component::handleResize(const sf::Event::SizeEvent& e) {
    return delegateToChildren([e](auto& child){return child.handleResize(e);});
}
