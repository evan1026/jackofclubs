#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "World/Block.h"

/*! \callergraph
 *
 * Constructs a block
 *
 * \p position - bottom left back corner                          <br>
 * \p color    - block color                                      <br>
 * \p type     - Either Block::Type::SOLID or Block::Type::AIR    <br>
 */
Block::Block(const sf::Vector3i& position, const sf::Color& color, const Type& type) : _position(position), _color(color), _type(type) {}

/*! \callergraph
 * Default constructor just because std::array needs default constructable objects
 */
Block::Block() : Block(sf::Vector3i(0,0,0), sf::Color(0,0,0), Type(Type::AIR)) {}

/*! \callergraph
 * Returns the bottom left back corner
 */
sf::Vector3i Block::getPosition() const {
    return _position;
}

/*! \callergraph
 *
 * Moving a block doesn't make much sense, but might as well fill out the API
 *
 * \p position - new bottom left back corner
 */
void Block::setPosition(const sf::Vector3i& position){
    _position = position;
}

/*! \callergraph
 * Returns the block's color
 */
sf::Color Block::getColor() const {
    return _color;
}

/*! \callergraph
 * \p color - new block color
 */
void Block::setColor(const sf::Color& color) {
    _color = color;
}

/*! \callergraph
 * Returns the block's Block::Type
 */
Block::Type Block::getType() const {
    return _type;
}

/*! \callergraph
 * \p t - new Block::Type
 */
void Block::setType(const Block::Type& t) {
    _type = t;
}

/*! \callergraph
 * Returns AABB with size (0,0,0) if air (indicates that it can be passed through),
 * or one with size (1,1,1) otherwise
 */
AABB Block::getBoundingBox() const {
    if (_type == Type::AIR) {
        return AABB(sf::Vector3f(_position.x, _position.y, _position.z), sf::Vector3f(0, 0, 0));
    }
    return AABB(sf::Vector3f(_position.x, _position.y, _position.z), sf::Vector3f(1, 1, 1));
}
