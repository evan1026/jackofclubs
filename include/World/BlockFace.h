#ifndef BLOCK_FACE_H
#define BLOCK_FACE_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Utils/EnumClassHash.h"

/*!
 * Represents one particular face of a block
 * Only keeps track of the block's position, rather
 * than keeping a reference to the block
 */
class BlockFace {

    public:
        /*! Face names */
        enum class Face {
            NORTH,
            SOUTH,
            EAST,
            WEST,
            TOP,
            BOTTOM
        };

    private:
        // Makes it a little easier to interface between BlockFace::Face's and
        // the normal vectors associated with them
        static std::unordered_map<Face, sf::Vector3f, EnumClassHash> faceNormals;
        void setFaceFromNormal(const sf::Vector3f& normal);

        sf::Vector3f _position;
        Face _face;
        sf::Vector3f _normal;
        sf::Color _color;

    public:
        BlockFace(const sf::Vector3f& position, const sf::Vector3f& normal, const sf::Color&);
        BlockFace(const sf::Vector3f& position, Face face, const sf::Color&);

        sf::Vector3f getPosition() const;
        sf::Vector3f getNormal() const;
        Face getFace() const;
        sf::Color getColor() const;

        void setPosition(const sf::Vector3f& position);
        void setNormal(const sf::Vector3f& normal);
        void setFace(Face face);
        void setColor(sf::Color color);
};

#endif
