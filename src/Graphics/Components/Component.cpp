#include "Graphics/Components/Component.h"

/*! \callergraph
 *
 * Constucts a new component based on its parent's position
 * and its position relative to the parent.
 *
 * \p localPos  - The position of the component relative to its parent                              <br>
 * \p parentPos - The parent's global position (given to the component by the parent)               <br>
 * \p size      - The length and width of the rectangular bounding box surrounding the component    <br>
 */
Component::Component(const sf::Vector2i& localPos, const sf::Vector2i& parentPos, const sf::Vector2i& size) :
    _boundingBox(parentPos + localPos, size),
    _localPos(localPos)
{}

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
sf::Vector2i Component::getSize() {
    return sf::Vector2i(_boundingBox.width, _boundingBox.height);
}

/*! \callergraph
 *
 * Returns the component's position relative to its parent
 */
sf::Vector2i Component::getLocalPosition() {
    return _localPos;
}

/*! \callergraph
 *
 * Returns the component's global position. Note that, unlike
 * setGlobalPosition(), this function is public
 */
sf::Vector2i Component::getGlobalPosition() {
    return sf::Vector2i(_boundingBox.left, _boundingBox.top);
}

/*! \callergraph
 *
 * Returns what the component thinks its parent's position is.
 * This may or may not be the parent's actual position.
 */
sf::Vector2i Component::getParentPosition() {
    return getGlobalPosition() - getLocalPosition();
}

/*! \callergraph
 *
 * Returns the bounding box of the component
 */
sf::Rect<int> Component::getBounds() {
    return _boundingBox;
}
