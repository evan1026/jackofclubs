#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "Block.h"
#include "RenderEngine.h"
#include "World.h"

const float Block::model[6][12] = {{-UNIT_LENGTH, -UNIT_LENGTH, -UNIT_LENGTH,
                                    -UNIT_LENGTH,  UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH,  UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH, -UNIT_LENGTH, -UNIT_LENGTH},
                                   {-UNIT_LENGTH, -UNIT_LENGTH,  UNIT_LENGTH,
                                    -UNIT_LENGTH,  UNIT_LENGTH,  UNIT_LENGTH,
                                     UNIT_LENGTH,  UNIT_LENGTH,  UNIT_LENGTH,
                                     UNIT_LENGTH, -UNIT_LENGTH,  UNIT_LENGTH},
                                   {-UNIT_LENGTH, -UNIT_LENGTH, -UNIT_LENGTH,
                                    -UNIT_LENGTH,  UNIT_LENGTH, -UNIT_LENGTH,
                                    -UNIT_LENGTH,  UNIT_LENGTH,  UNIT_LENGTH,
                                    -UNIT_LENGTH, -UNIT_LENGTH,  UNIT_LENGTH},
                                   { UNIT_LENGTH, -UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH,  UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH,  UNIT_LENGTH,  UNIT_LENGTH,
                                     UNIT_LENGTH, -UNIT_LENGTH,  UNIT_LENGTH},
                                   {-UNIT_LENGTH, -UNIT_LENGTH,  UNIT_LENGTH,
                                    -UNIT_LENGTH, -UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH, -UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH, -UNIT_LENGTH,  UNIT_LENGTH},
                                   {-UNIT_LENGTH,  UNIT_LENGTH,  UNIT_LENGTH,
                                    -UNIT_LENGTH,  UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH,  UNIT_LENGTH, -UNIT_LENGTH,
                                     UNIT_LENGTH,  UNIT_LENGTH,  UNIT_LENGTH}};

sf::Vector3i Block::getPosition() const {
    return _position;
}

void Block::setPosition(const sf::Vector3i& position){
    _position = position;
}

sf::Color Block::getColor() const {
    return _color;
}

void Block::setColor(const sf::Color& color) {
    _color = color;
}

Block::Type Block::getType() const {
    return _type;
}

void Block::setType(const Block::Type& t) {
    _type = t;
}

void Block::render(RenderEngine& e) const {
    if (_type == Type::AIR)
        return;

    e.startRender(sf::Vector3f(_position.x * SIZE, _position.y * SIZE, _position.z * SIZE));
    e.color(_color);

    World& world = World::getInst();

    if (world.getBlockType(sf::Vector3i(_position.x, _position.y, _position.z - 1)) == Type::AIR)
        renderSide(e, 0);
    if (world.getBlockType(sf::Vector3i(_position.x, _position.y, _position.z + 1)) == Type::AIR)
        renderSide(e, 1);
    if (world.getBlockType(sf::Vector3i(_position.x - 1, _position.y, _position.z)) == Type::AIR)
        renderSide(e, 2);
    if (world.getBlockType(sf::Vector3i(_position.x + 1, _position.y, _position.z)) == Type::AIR)
        renderSide(e, 3);
    if (world.getBlockType(sf::Vector3i(_position.x, _position.y - 1, _position.z)) == Type::AIR)
        renderSide(e, 4);
    if (world.getBlockType(sf::Vector3i(_position.x, _position.y + 1, _position.z)) == Type::AIR)
        renderSide(e, 5);

    e.endRender();
}

void Block::renderSide(RenderEngine& e, int sidenum) const {
    for (int j = 0; j < 12; j += 3) {
        e.vertex(sf::Vector3f(model[sidenum][j], model[sidenum][j + 1], model[sidenum][j + 2]));
    }
}
