#include <cmath>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Exception/NullptrException.h"
#include "Rendering/RenderEngine.h"
#include "Rendering/IRenderable.h"

#define PI 3.1415926535897932384626433832795

RenderEngine* RenderEngine::inst = nullptr;

RenderEngine::RenderEngine() {
    sf::ContextSettings settings(24, 0, 0, 1, 1, sf::ContextSettings::Attribute::Debug);
    window = std::shared_ptr<sf::Window>(new sf::Window(sf::VideoMode(1920,1080), "jack o' clubs", sf::Style::Default, settings));
    window->setVerticalSyncEnabled(true);
}

void RenderEngine::init() {
    if (inst == nullptr) {
        inst = new RenderEngine();
    }
}

void RenderEngine::end() {
    if (inst != nullptr) {
        delete inst;
    }
}

void RenderEngine::setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * PI ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

RenderEngine& RenderEngine::getInst() {
    if (inst == nullptr) {
        throw NullptrException();
    }
    return *inst;
}

void RenderEngine::addRenderable(IRenderable& object) {
    renderables.insert(&object);
}

void RenderEngine::removeRenderable(IRenderable& object) {
    auto search = renderables.find(&object);
    if (search != renderables.end()) {
        renderables.erase(search);
    }
}

void RenderEngine::render() {
    for (auto i = renderables.begin(); i != renderables.end(); i++) {
        (*i)->render(*this);
    }
}

void RenderEngine::pushMatrix() {
    glPushMatrix();
}

void RenderEngine::popMatrix() {
    glPopMatrix();
}

void RenderEngine::startRender(const sf::Vector3f& position) {
    pushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glBegin(GL_QUADS);
}

void RenderEngine::color(const sf::Color& color) {
    glColor3f(color.r / 256.f, color.g / 256.f, color.b / 256.f);
}

void RenderEngine::vertex(const sf::Vector3f& position) {
    glVertex3f(position.x, position.y, position.z);
}

void RenderEngine::endRender() {
    glEnd();
    popMatrix();
}
