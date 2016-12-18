#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#include "Logger/GlobalLogger.hpp"
#include "Player.h"
#include "Rendering/RenderEngine.h"
#include "Rendering/Vertex.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
#include "World/Block.h"

using Logger::globalLogger;

//TODO UPgrade to modern OpenGL

// RGBA values for different lights
float RenderEngine::lightPos[] =     { -0.1f, -1.0f, 0.2f, 0.f  };
float RenderEngine::lightAmbient[] = {  0.5f,  0.5f, 0.5f, 1.0f };
float RenderEngine::lightDiffuse[] = {  0.7f,  0.7f, 0.7f, 1.0f};

/*
 * Creates a rendering window and sets up OpenGL
 */
RenderEngine::RenderEngine() :
    _window(getVideoMode(),
            "jack o' clubs",
            sf::Style::Default,
            sf::ContextSettings(24))
{
    sf::Vector2u windowSize = _window.getSize();
    sf::ContextSettings windowSettings = _window.getSettings();

    // This code centers the window on the screen
    sf::VideoMode defaultVideoMode = sf::VideoMode::getDesktopMode();
    _window.setPosition(sf::Vector2i(defaultVideoMode.width / 2 - windowSize.x / 2, defaultVideoMode.height / 2 - windowSize.y / 2));
    sf::Vector2i windowPosition = _window.getPosition();

    globalLogger.log("Initialized window:");
    globalLogger.log("    OS Handle:           ", _window.getSystemHandle());
    globalLogger.log("    Size:                ", windowSize.x, "x", windowSize.y);
    globalLogger.log("    Position:            (", windowPosition.x, ", ", windowPosition.y, ")");
    globalLogger.log("    Depth bits:          ", windowSettings.depthBits);
    globalLogger.log("    Stencil bits:        ", windowSettings.stencilBits);
    globalLogger.log("    Antialiasing level:  ", windowSettings.antialiasingLevel);
    globalLogger.log("    GL version:          ", windowSettings.majorVersion, ".", windowSettings.minorVersion);
    globalLogger.log("    Attribute flags:     ", windowSettings.attributeFlags);
    globalLogger.log("    sRGB capable:        ", windowSettings.sRgbCapable ? "true" : "false");

    _window.setVerticalSyncEnabled(true);

    // Set background color
    glClearDepth(1.f);
    glClearColor(217.f / 256.f, 233.f / 256.f, 255.f / 256.f, 1.f);

    // Make sure things in front get drawn in front
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Don't draw both sides of the face (more efficient)
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);

    // Turn on lighting and make a vertexes color based on
    // glColor rather than glMaterial
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // TODO attempt 2 light system so all faces are different

    // Set up our "sun"
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glEnable(GL_LIGHT0);

    // Make sure the perspective matches the window
    setPerspective(windowSize.x, windowSize.y);
}

/*
 * Returns a video mode where the window takes up 1/4 of the screen
 */
sf::VideoMode RenderEngine::getVideoMode() {
    sf::VideoMode defaultVideoMode = sf::VideoMode::getDesktopMode();
    return sf::VideoMode(defaultVideoMode.width / 2, defaultVideoMode.height / 2, defaultVideoMode.bitsPerPixel);
}

/*
 * Defines default values for the other setPerspective
 *
 * width  - width of the window
 * height - height of the window
 */
void RenderEngine::setPerspective(int width, int height) {
    setPerspective(60.f, width, height, 1.f, 10000.f);
}

/*
 * Calls glFustrum with the appropriate values give the input parameters.
 * idk how glFustrum works, but this code is from StackOverflow, so it's probably right.
 *
 * fovY   - Desired field of view
 * width  - Window width
 * height - Window height
 * zNear  - How close something has to be to be clipped
 * zFar   - How far something has to be to be clipped
 */
void RenderEngine::setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar) {
    GLdouble aspect = (double) width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    GLdouble fW, fH;

    fH = tan( fovY / 360 * Math::PI ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

/*
 * Handles notifying OpenGL and SFML's OpenGL that the screen has
 * been resized.
 *
 * width  - new window width
 * height - new window height
 */
void RenderEngine::handleResize(int width, int height) {
    setPerspective(width, height);

    // Update's the window's internal GL states to match the resize
    _window.setView(sf::View(sf::FloatRect(0, 0, width, height)));

    globalLogger.log("Window resized to ", width, "x", height);
}

/*
 * Called before doing a full render. Sets OpenGL up to do rendering.
 */
void RenderEngine::beginRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Called after doing a full render. Flushes the buffer to the screen.
 */
void RenderEngine::endRender() {
    _window.display();
}

/*
 * Moves the camera based on the player's position. Automatically handles
 * moving the camera to the player's head (whereas their position is in their feet).
 *
 * position - Player's position
 */
void RenderEngine::translatePlayer(const Player& player) {
    sf::Vector3f position = player.getHeadLocation();
    glTranslatef(-position.x * SCALE, -position.y * SCALE, -position.z * SCALE);
}

/*
 * Rotates the camera based on the direction the player is looking
 *
 * rotation - Player's look direction
 */
void RenderEngine::rotatePlayer(const Player& player) {
    sf::Vector3f rotation = player.getRotation();
    glRotatef(rotation.x, 1.f, 0.f, 0.f);
    glRotatef(rotation.y, 0.f, 1.f, 0.f);
    glRotatef(rotation.z, 0.f, 0.f, 1.f);
}

/*
 * Renders an entire array of vertices, all at once
 *
 * vertices - The vertices to render
 */
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

/*
 * Given a top left front point, pushes all of the vertices needed to render a
 * block.
 *
 * p - Starting position
 * s - Size (in each direction)
 */
void RenderEngine::pushBlockVertices(const sf::Vector3f& p, const sf::Vector3f& s) {
    sf::Vector3f position = p;
    sf::Vector3f size = s;

    if (size.x < 0) {
        position.x += size.x; // Position will get smaller since size is negative
        size.x = -size.x;
    }
    if (size.y < 0) {
        position.y += size.y; // Position will get smaller since size is negative
        size.y = -size.y;
    }
    if (size.z < 0) {
        position.z += size.z; // Position will get smaller since size is negative
        size.z = -size.z;
    }

    glVertex3f(position.x,          position.y, position.z);
    glVertex3f(position.x,          position.y, position.z + size.z);
    glVertex3f(position.x + size.x, position.y, position.z + size.z);
    glVertex3f(position.x + size.x, position.y, position.z);

    glVertex3f(position.x,          position.y,          position.z);
    glVertex3f(position.x + size.x, position.y,          position.z);
    glVertex3f(position.x + size.x, position.y + size.y, position.z);
    glVertex3f(position.x,          position.y + size.y, position.z);

    glVertex3f(position.x, position.y,          position.z);
    glVertex3f(position.x, position.y + size.y, position.z);
    glVertex3f(position.x, position.y + size.y, position.z + size.z);
    glVertex3f(position.x, position.y,          position.z + size.z);

    glVertex3f(position.x + size.x, position.y + size.y, position.z + size.z);
    glVertex3f(position.x,          position.y + size.y, position.z + size.z);
    glVertex3f(position.x,          position.y + size.y, position.z);
    glVertex3f(position.x + size.x, position.y + size.y, position.z);

    glVertex3f(position.x + size.x, position.y + size.y, position.z + size.z);
    glVertex3f(position.x + size.x, position.y,          position.z + size.z);
    glVertex3f(position.x,          position.y,          position.z + size.z);
    glVertex3f(position.x,          position.y + size.y, position.z + size.z);

    glVertex3f(position.x + size.x, position.y + size.y, position.z + size.z);
    glVertex3f(position.x + size.x, position.y + size.y, position.z);
    glVertex3f(position.x + size.x, position.y,          position.z);
    glVertex3f(position.x + size.x, position.y,          position.z + size.z);
}

/*
 * Renders an axis aligned bounding box. Simply turns off culling
 * and makes rendering use lines instead of fill, then calls
 * pushBlockVertcies() (gotta turn off culling because pushBlockVerticies()
 * is designed so that it renders only the outside of the block,
 * but you can be inside the AABB)
 *
 * box   - The AABB to render
 * color - The color to render the AABB
 */
void RenderEngine::renderAABB(const AABB& box, const sf::Color& color) {
    sf::Vector3f p = box.getPosition();
    sf::Vector3f s = box.getSize();

    s = s * SCALE;
    p = p * SCALE;

    // Save state
    glPushMatrix();

    // Turn off culling
    glDisable(GL_CULL_FACE);

    // Make it line mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);

    // Interpret vertices in 4's
    glBegin(GL_QUADS);

    // Set the color
    glColor3f(color.r / 256.f, color.g / 256.f, color.b / 256.f);

    // Push the vertices
    pushBlockVertices(p, s);

    // Done pushing vertices
    glEnd();

    // Turn fill back on
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Turn culling back on
    glEnable(GL_CULL_FACE);

    // Pop old state
    glPopMatrix();
}

#define LINE_WIDTH 0.05

/*
 * Render the box around a block selection. Similar to renderAABB()
 * except that it renders 1 block for each edge instead of lines, where
 * each rendered block is LINE_WIDTH x LINE_WIDTH x 1. Essentially, it looks
 * like a line, but it won't get completely cut off when blocks are next to
 * each other (which was the observed behavior when using lines)
 *
 * box   - The AABB to render
 * color - The color to use in rendering
 */
void RenderEngine::renderBlockSelection(const AABB& box, const sf::Color& color) {
    sf::Vector3f p = box.getPosition();
    sf::Vector3f s = box.getSize();
    sf::Vector3f linePos;
    sf::Vector3f lineSize;

    // Scale to GL coords
    s = s * SCALE;
    p = p * SCALE;

    glPushMatrix();

        glBegin(GL_QUADS);

            glColor3f(color.r / 256.f, color.g / 256.f, color.b / 256.f);

            // Now, we go through the points on the block and render the edges that haven't been taken care of
            // Each block is one point, whereas each call to pushBlockVertices is an edge that ends at that point

            linePos = p - sf::Vector3f(LINE_WIDTH / 2, LINE_WIDTH / 2, LINE_WIDTH / 2);
            lineSize = sf::Vector3f(LINE_WIDTH, LINE_WIDTH, s.z + LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(LINE_WIDTH, s.y + LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(s.x + LINE_WIDTH, LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x, p.y, p.z + SCALE) - sf::Vector3f(LINE_WIDTH / 2, LINE_WIDTH / 2, -LINE_WIDTH / 2);
            lineSize = sf::Vector3f(LINE_WIDTH, s.y + LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(s.x + LINE_WIDTH, LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x, p.y + SCALE, p.z) - sf::Vector3f(LINE_WIDTH / 2, -LINE_WIDTH / 2, LINE_WIDTH / 2);
            lineSize = sf::Vector3f(LINE_WIDTH, -LINE_WIDTH, s.z + LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(s.x + LINE_WIDTH, -LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x + SCALE, p.y, p.z) - sf::Vector3f(-LINE_WIDTH / 2, LINE_WIDTH / 2, LINE_WIDTH / 2);
            lineSize = sf::Vector3f(-LINE_WIDTH, s.y + LINE_WIDTH, LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(-LINE_WIDTH, LINE_WIDTH, s.z + LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

            linePos = sf::Vector3f(p.x + SCALE, p.y + SCALE, p.z + SCALE)
                        - sf::Vector3f(-LINE_WIDTH / 2, -LINE_WIDTH / 2, -LINE_WIDTH / 2);
            lineSize = sf::Vector3f(-LINE_WIDTH, -LINE_WIDTH, -s.z - LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(-LINE_WIDTH, -s.y - LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);
            lineSize = sf::Vector3f(-s.x - LINE_WIDTH, -LINE_WIDTH, -LINE_WIDTH);
            pushBlockVertices(linePos, lineSize);

        glEnd();

    glPopMatrix();
}

/*
 * Returns the render window. Useful for making SFML calls.
 */
sf::RenderWindow& RenderEngine::getWindow() {
    return _window;
}
