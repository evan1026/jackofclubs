#ifndef BLOCK_FACE_H
#define BLOCK_FACE_H

#include <SFML/System.hpp>
#include <unordered_map>

#include "Utils/EnumClassHash.h"

class BlockFace {

    public:
        enum class Face {
            NORTH,
            SOUTH,
            EAST,
            WEST,
            TOP,
            BOTTOM
        };

    private:
        static std::unordered_map<Face, sf::Vector3f, EnumClassHash> faceNormals;
        void setFaceFromNormal(const sf::Vector3f& normal);

        sf::Vector3f _position;
        Face _face;
        sf::Vector3f _normal;

    public:
        BlockFace(const sf::Vector3f& position, const sf::Vector3f& normal);
        BlockFace(const sf::Vector3f& position, Face face);
        BlockFace();

        sf::Vector3f getPosition() const;
        sf::Vector3f getNormal() const;
        Face getFace() const;

        void setPosition(const sf::Vector3f& position);
        void setNormal(const sf::Vector3f& normal);
        void setFace(Face face);
};

#endif
