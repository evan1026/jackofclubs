#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "Utils/ICollidable.h"
#include "Utils/AABB.h"

/*!
 * It's a block. Seems pretty obvious.
 */
class Block : public ICollidable {
    public:
        /*! Different states the block can take on */
        enum class Type {
            AIR,
            SOLID
        };

    private:
        sf::Vector3i _position;
        sf::Color _color;
        Type _type;

    public:
        Block(const sf::Vector3i& position, const sf::Color& color, const Type& type);
        Block();

        sf::Vector3i getPosition() const;
        void setPosition(const sf::Vector3i& position);

        sf::Color getColor() const;
        void setColor(const sf::Color& color);

        Type getType() const;
        void setType(const Type& t);

        AABB getBoundingBox() const override;
};

#endif
