#include <cmath>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "NullptrException.h"
#include "RenderEngine.h"
#include "IRenderable.h"

#define PI 3.1415926535897932384626433832795

RenderEngine* RenderEngine::inst = nullptr;

RenderEngine::RenderEngine() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    window = std::shared_ptr<sf::Window>(new sf::Window(sf::VideoMode(800,600), "jack o' clubs", sf::Style::Default, settings));
    window->setVerticalSyncEnabled(true);
}

void RenderEngine::init() {
    inst = new RenderEngine();
}

void RenderEngine::end() {
    delete inst;
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
