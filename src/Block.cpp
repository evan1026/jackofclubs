#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "Block.h"
#include "RenderEngine.h"

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

void Block::render(RenderEngine& e) const {
    e.startRender(sf::Vector3f(_position.x * SIZE, _position.y * SIZE, _position.z * SIZE));

    for (int i = 0; i < 6; ++i) {
        e.color(_color);

        for (int j = 0; j < 12; j += 3) {
            e.vertex(sf::Vector3f(model[i][j], model[i][j + 1], model[i][j + 2]));
        }
    }

    e.endRender();
}
