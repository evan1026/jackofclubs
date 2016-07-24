#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "Block.h"


const float Block::model[6][12] = {{-50.f, -50.f, -50.f,
                                    -50.f,  50.f, -50.f,
                                     50.f,  50.f, -50.f,
                                     50.f, -50.f, -50.f},
                                   {-50.f, -50.f,  50.f,
                                    -50.f,  50.f,  50.f,
                                     50.f,  50.f,  50.f,
                                     50.f, -50.f,  50.f},
                                   {-50.f, -50.f, -50.f,
                                    -50.f,  50.f, -50.f,
                                    -50.f,  50.f,  50.f,
                                    -50.f, -50.f,  50.f},
                                   { 50.f, -50.f, -50.f,
                                     50.f,  50.f, -50.f,
                                     50.f,  50.f,  50.f,
                                     50.f, -50.f,  50.f},
                                   {-50.f, -50.f,  50.f,
                                    -50.f, -50.f, -50.f,
                                     50.f, -50.f, -50.f,
                                     50.f, -50.f,  50.f},
                                   {-50.f,  50.f,  50.f,
                                    -50.f,  50.f, -50.f,
                                     50.f,  50.f, -50.f,
                                     50.f,  50.f,  50.f}};

sf::Vector3f Block::getPosition() const {
    return _position;
}

void Block::setPosition(const sf::Vector3f& position){
    _position = position;
}

sf::Color Block::getColor() const {
    return _color;
}

void Block::setColor(const sf::Color& color) {
    _color = color;
}

void Block::render(RenderEngine& e) const {
    glPushMatrix();
    glTranslatef(_position.x, _position.y, _position.z);
    glBegin(GL_QUADS);

    for (int i = 0; i < 6; ++i) {
        glColor3f(_color.r / 256.f, _color.g / 256.f, _color.b / 256.f);

        for (int j = 0; j < 12; j += 3) {
            glVertex3f(model[i][j], model[i][j + 1], model[i][j + 2]);
        }
    }

    glEnd();
    glPopMatrix();
}
