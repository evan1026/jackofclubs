#include <cmath>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Exception/NullptrException.h"
#include "Rendering/RenderEngine.h"
#include "Rendering/IRenderable.h"

#define PI 3.1415926535897932384626433832795

RenderEngine* RenderEngine::inst = nullptr;

RenderEngine::RenderEngine(int width, int height) {
    sf::ContextSettings settings(24);
    window = std::shared_ptr<sf::Window>(new sf::Window(sf::VideoMode(width, height), "jack o' clubs", sf::Style::Default, settings));
    window->setVerticalSyncEnabled(true);
}

void RenderEngine::init(int width, int height) {
    if (inst == nullptr) {
        inst = new RenderEngine(width, height);
    }
}

void RenderEngine::end() {
    if (inst != nullptr) {
        delete inst;
    }
}

//Code from stackoverflow
void RenderEngine::setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    GLdouble fW, fH;

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
    _renderables.insert(&object);
}

void RenderEngine::removeRenderable(IRenderable& object) {
    auto search = _renderables.find(&object);
    if (search != _renderables.end()) {
        _renderables.erase(search);
    }
}

void RenderEngine::render() {
    for (auto i = _renderables.begin(); i != _renderables.end(); i++) {
        (*i)->render(*this);
    }
}

void RenderEngine::renderVertexArray(const std::vector<Vertex>& vertices) {
    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].pos[0]);
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].color[0]);

    glDrawArrays(GL_QUADS, 0, vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}
