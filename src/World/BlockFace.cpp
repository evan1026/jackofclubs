#include "BlockFace.h"
#include "Utils/EnumClassHash.h"

/*!
 * Defines normal vectors for each of the possible faces
 */
std::unordered_map<BlockFace::Face, sf::Vector3f, EnumClassHash> BlockFace::faceNormals =
    {
        { BlockFace::Face::NORTH,  sf::Vector3f( 0,  0,  1) },
        { BlockFace::Face::SOUTH,  sf::Vector3f( 0,  0, -1) },
        { BlockFace::Face::EAST,   sf::Vector3f( 1,  0,  0) },
        { BlockFace::Face::WEST,   sf::Vector3f(-1,  0,  0) },
        { BlockFace::Face::TOP,    sf::Vector3f( 0,  1,  0) },
        { BlockFace::Face::BOTTOM, sf::Vector3f( 0, -1,  0) }
    };

/*! \callergraph
 *
 * Creates a BlockFace based on its normal vector
 *
 * \p position - Block's position                             <br>
 * \p normal   - Normal vector pointing away from the face    <br>
 */
BlockFace::BlockFace(const sf::Vector3f& position, const sf::Vector3f& normal, const sf::Color& color) :
    _position(position),
    _normal(normal),
    _color(color)
{
    setFaceFromNormal(_normal);
}

/*! \callergraph
 *
 * Creates a BlockFace based on the face name
 *
 * \p position - Block's position    <br>
 * \p face     - Name of the face    <br>
 */
BlockFace::BlockFace(const sf::Vector3f& position, BlockFace::Face face, const sf::Color& color) :
    _position(position),
    _face(face),
    _normal(faceNormals[face]),
    _color(color)
{}

/*! \callergraph
 * Returns the position of the block this face is on
 */
sf::Vector3f BlockFace::getPosition() const {
    return _position;
}

/*! \callergraph
 * Returns the normal vector pointing away from this face
 */
sf::Vector3f BlockFace::getNormal() const {
    return _normal;
}

/*! \callergraph
 * Returns the BlockFace::Face associated with this face
 */
BlockFace::Face BlockFace::getFace() const {
    return _face;
}

sf::Color BlockFace::getColor() const {
    return _color;
}

/*! \callergraph
 * Sets the block this face is on
 */
void BlockFace::setPosition(const sf::Vector3f& position) {
    _position = position;
}

/*! \callergraph
 * Sets the normal pointing away from this face
 */
void BlockFace::setNormal(const sf::Vector3f& normal) {
    _normal = normal;
    setFaceFromNormal(normal);
}

/*! \callergraph
 * Sets the face for this face
 */
void BlockFace::setFace(BlockFace::Face face) {
    _face = face;
    _normal = faceNormals[face];
}

void BlockFace::setColor(sf::Color color) {
    _color = color;
}

/*! \callergraph
 *
 * Utility function to get the BlockFace::Face from the normal vector.
 * Goes through all of them and returns the one that matches. Not a
 * big deal since there's only 6
 */
void BlockFace::setFaceFromNormal(const sf::Vector3f& normal) {
    for (auto i = faceNormals.begin(); i != faceNormals.end(); i++) {
        if (i->second == normal) {
            _face = i->first;
            break;
        }
    }
}
