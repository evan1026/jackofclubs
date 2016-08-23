#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#include "Exception/AlreadyInitializedException.h"
#include "Exception/NotYetInitializedException.h"
#include "Rendering/IRenderable.h"
#include "Rendering/RenderEngine.h"
#include "Rendering/Vertex.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"

RenderEngine* RenderEngine::inst = nullptr;

RenderEngine::RenderEngine(int width, int height)
        : _window(sf::VideoMode(width, height),
                  "jack o' clubs",
                  sf::Style::Default,
                  sf::ContextSettings(24))
{
    _window.setVerticalSyncEnabled(true);

    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    setPerspective(60.f, _window.getSize().x, _window.getSize().y, 1.f, 10000.f);

    if (inst != nullptr) {
        throw AlreadyInitializedException();
    } else {
        inst = this;
    }
}

RenderEngine& RenderEngine::getInst() {
    if (inst == nullptr) {
        throw NotYetInitializedException();
    }
    return *inst;
}

//Code from stackoverflow
void RenderEngine::setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar) {
    GLdouble aspect = (double) width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, _window.getSize().x, _window.getSize().y);

    GLdouble fW, fH;

    fH = tan( fovY / 360 * Math::PI ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
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

void RenderEngine::handleResize(int width, int height) {
    glViewport(0, 0, width, height);
    setPerspective(60.f, width, height, 1.f, 10000.f);
}

void RenderEngine::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (auto i = _renderables.begin(); i != _renderables.end(); i++) {
        (*i)->render(*this);
    }

    _window.display();
}

void RenderEngine::translatePlayer(const sf::Vector3f& position) {
    glTranslatef(-(position.x + 0.5) * 30, -(position.y + 1.75)  * 30, -(position.z + 0.5) * 30);
}

void RenderEngine::rotatePlayer(const sf::Vector3f& rotation) {
    glRotatef(rotation.x, 1.f, 0.f, 0.f);
    glRotatef(rotation.y, 0.f, 1.f, 0.f);
    glRotatef(rotation.z, 0.f, 0.f, 1.f);
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

void RenderEngine::renderAABB(const AABB& box, const sf::Color& color) {
    sf::Vector3f p = box.getPosition();
    sf::Vector3f s = box.getSize();

    s = s * 30.f;
    p = p * 30.f;

    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_QUADS);

    glColor3f(color.r / 256.f, color.g / 256.f, color.b / 256.f);

    glVertex3f(p.x,       p.y, p.z);
    glVertex3f(p.x + s.x, p.y, p.z);
    glVertex3f(p.x + s.x, p.y, p.z + s.z);
    glVertex3f(p.x,       p.y, p.z + s.z);

    glVertex3f(p.x,       p.y,       p.z);
    glVertex3f(p.x + s.x, p.y,       p.z);
    glVertex3f(p.x + s.x, p.y + s.y, p.z);
    glVertex3f(p.x,       p.y + s.y, p.z);

    glVertex3f(p.x, p.y,       p.z);
    glVertex3f(p.x, p.y + s.y, p.z);
    glVertex3f(p.x, p.y + s.y, p.z + s.z);
    glVertex3f(p.x, p.y,       p.z + s.z);

    glVertex3f(p.x + s.x, p.y + s.y, p.z + s.z);
    glVertex3f(p.x,       p.y + s.y, p.z + s.z);
    glVertex3f(p.x,       p.y + s.y, p.z);
    glVertex3f(p.x + s.x, p.y + s.y, p.z);

    glVertex3f(p.x + s.x, p.y + s.y, p.z + s.z);
    glVertex3f(p.x,       p.y + s.y, p.z + s.z);
    glVertex3f(p.x,       p.y,       p.z + s.z);
    glVertex3f(p.x + s.x, p.y,       p.z + s.z);

    glVertex3f(p.x + s.x, p.y + s.y, p.z + s.z);
    glVertex3f(p.x + s.x, p.y,       p.z + s.z);
    glVertex3f(p.x + s.x, p.y,       p.z);
    glVertex3f(p.x + s.x, p.y + s.y, p.z);

    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPopMatrix();
}

sf::Window& RenderEngine::getWindow() {
    return _window;
}
