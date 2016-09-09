#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#include "Rendering/RenderEngine.h"
#include "Rendering/Vertex.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
#include "World/Block.h"

float RenderEngine::lightPos[] =     { -0.1f, -1.0f, 0.2f, 0.f  };
float RenderEngine::lightAmbient[] = {  0.5f,  0.5f, 0.5f, 1.0f };
float RenderEngine::lightDiffuse[] = {  0.7f,  0.7f, 0.7f, 1.0f};

RenderEngine::RenderEngine() :
    _window(getVideoMode(),
            "jack o' clubs",
            sf::Style::Default,
            sf::ContextSettings(24))
{
    _window.setVerticalSyncEnabled(true);

    glClearDepth(1.f);
    glClearColor(217.f / 256.f, 233.f / 256.f, 255.f / 256.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glEnable(GL_LIGHT0);

    sf::Vector2u windowSize = _window.getSize();

    setPerspective(windowSize.x, windowSize.y);

    // This code centers the window on the screen
    sf::VideoMode defaultVideoMode = sf::VideoMode::getDesktopMode();
    _window.setPosition(sf::Vector2i(defaultVideoMode.width / 2 - windowSize.x / 2, defaultVideoMode.height / 2 - windowSize.y / 2));
}

// Returns a video mode where the window takes up 1/4 of the screen
sf::VideoMode RenderEngine::getVideoMode() {
    sf::VideoMode defaultVideoMode = sf::VideoMode::getDesktopMode();
    return sf::VideoMode(defaultVideoMode.width / 2, defaultVideoMode.height / 2, defaultVideoMode.bitsPerPixel);
}

void RenderEngine::setPerspective(int width, int height) {
    setPerspective(60.f, width, height, 1.f, 10000.f);
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

void RenderEngine::handleResize(int width, int height) {
    glViewport(0, 0, width, height);
    setPerspective(width, height);

    // Update's the window's internal GL states to match the resize
    _window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
}

void RenderEngine::beginRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderEngine::endRender() {
    _window.display();
}

void RenderEngine::translatePlayer(const sf::Vector3f& position) {
    glTranslatef(-(position.x + 0.5) * Block::SIZE, -(position.y + 1.75)  * Block::SIZE, -(position.z + 0.5) * Block::SIZE);
}

void RenderEngine::rotatePlayer(const sf::Vector3f& rotation) {
    glRotatef(rotation.x, 1.f, 0.f, 0.f);
    glRotatef(rotation.y, 0.f, 1.f, 0.f);
    glRotatef(rotation.z, 0.f, 0.f, 1.f);
}

void RenderEngine::renderVertexArray(const std::vector<Vertex>& vertices) {
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].pos[0]);
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].color[0]);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), &vertices[0].normal[0]);

    glDrawArrays(GL_QUADS, 0, vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void RenderEngine::pushBlockVertices(const sf::Vector3f& p, const sf::Vector3f& s) {
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
}

void RenderEngine::renderAABB(const AABB& box, const sf::Color& color) {
    sf::Vector3f p = box.getPosition();
    sf::Vector3f s = box.getSize();

    s = s * Block::SIZE;
    p = p * Block::SIZE;

    glPushMatrix();

    glDisable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);

    glBegin(GL_QUADS);

    glColor3f(color.r / 256.f, color.g / 256.f, color.b / 256.f);

    pushBlockVertices(p, s);

    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

#define LINE_WIDTH 0.05
void RenderEngine::renderBlockSelection(const AABB& box, const sf::Color& color) {
    sf::Vector3f p = box.getPosition();
    sf::Vector3f s = box.getSize();
    sf::Vector3f linePos;
    sf::Vector3f lineSize;

    s = s * Block::SIZE;
    p = p * Block::SIZE;

    glPushMatrix();

        glDisable(GL_CULL_FACE);

        glBegin(GL_QUADS);

            glColor3f(color.r / 256.f, color.g / 256.f, color.b / 256.f);

            linePos = p - sf::Vector3f(LINE_WIDTH / 2, LINE_WIDTH / 2, LINE_WIDTH / 2);
            lineSize = sf::Vector3f(LINE_WIDTH, LINE_WIDTH, s.z + LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(LINE_WIDTH, s.y + LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(s.x + LINE_WIDTH, LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x, p.y, p.z + Block::SIZE) - sf::Vector3f(LINE_WIDTH / 2, LINE_WIDTH / 2, -LINE_WIDTH / 2);
            lineSize = sf::Vector3f(LINE_WIDTH, s.y + LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(s.x + LINE_WIDTH, LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x, p.y + Block::SIZE, p.z) - sf::Vector3f(LINE_WIDTH / 2, -LINE_WIDTH / 2, LINE_WIDTH / 2);
            lineSize = sf::Vector3f(LINE_WIDTH, -LINE_WIDTH, s.z + LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(s.x + LINE_WIDTH, -LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x + Block::SIZE, p.y, p.z) - sf::Vector3f(-LINE_WIDTH / 2, LINE_WIDTH / 2, LINE_WIDTH / 2);
            lineSize = sf::Vector3f(-LINE_WIDTH, s.y + LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(-LINE_WIDTH, LINE_WIDTH, s.z + LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x + Block::SIZE, p.y + Block::SIZE, p.z + Block::SIZE)
                        - sf::Vector3f(-LINE_WIDTH / 2, -LINE_WIDTH / 2, -LINE_WIDTH / 2);
            lineSize = sf::Vector3f(-LINE_WIDTH, -LINE_WIDTH, -s.z - LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(-LINE_WIDTH, -s.y - LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(-s.x - LINE_WIDTH, -LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

        glEnd();

        glEnable(GL_CULL_FACE);

    glPopMatrix();
}

sf::RenderWindow& RenderEngine::getWindow() {
    return _window;
}
