#include "BlockFace.h"
#include "Utils/EnumClassHash.h"

std::unordered_map<BlockFace::Face, sf::Vector3f, EnumClassHash> BlockFace::faceNormals =
    {
        { BlockFace::Face::NORTH,  sf::Vector3f( 0,  0,  1) },
        { BlockFace::Face::SOUTH,  sf::Vector3f( 0,  0, -1) },
        { BlockFace::Face::EAST,   sf::Vector3f( 1,  0,  0) },
        { BlockFace::Face::WEST,   sf::Vector3f(-1,  0,  0) },
        { BlockFace::Face::TOP,    sf::Vector3f( 0,  1,  0) },
        { BlockFace::Face::BOTTOM, sf::Vector3f( 0, -1,  0) }
    };

BlockFace::BlockFace(const sf::Vector3f& position, const sf::Vector3f& normal) :
    _position(position),
    _normal(normal)
{
    setFaceFromNormal(_normal);
}

BlockFace::BlockFace(const sf::Vector3f& position, BlockFace::Face face) :
    _position(position),
    _face(face),
    _normal(faceNormals[face])
{}

sf::Vector3f BlockFace::getPosition() const {
    return _position;
}

sf::Vector3f BlockFace::getNormal() const {
    return _normal;
}

BlockFace::Face BlockFace::getFace() const {
    return _face;
}

void BlockFace::setPosition(const sf::Vector3f& position) {
    _position = position;
}

void BlockFace::setNormal(const sf::Vector3f& normal) {
    _normal = normal;
    setFaceFromNormal(normal);
}

void BlockFace::setFace(BlockFace::Face face) {
    _face = face;
    _normal = faceNormals[face];
}

void BlockFace::setFaceFromNormal(const sf::Vector3f& normal) {
    for (auto i = faceNormals.begin(); i != faceNormals.end(); i++) {
        if (i->second == normal) {
            _face = i->first;
            break;
        }
    }
}
