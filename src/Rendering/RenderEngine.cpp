#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Logger/GlobalLogger.hpp"
#include "Player.h"
#include "Rendering/RenderEngine.h"
#include "Rendering/Vertex.h"
#include "Utils/AABB.h"
#include "Utils/Math.h"
#include "World/Block.h"

using Logger::globalLogger;

// RGBA values for different lights
float RenderEngine::lightPos[] =     { -0.1f,  -1.0f,   0.2f,  0.f  };
float RenderEngine::light2Pos[] =    {  0.1f,  -1.0f,  -0.2f,  0.f  };
float RenderEngine::lightAmbient[] = {  0.25f,  0.25f,  0.25f, 1.0f };
float RenderEngine::lightDiffuse[] = {  0.35f,  0.35f,  0.35f, 1.0f };

/*! \callergraph
 *
 * Creates a rendering window and sets up OpenGL
 */
RenderEngine::RenderEngine() :
    _window(getVideoMode(),
            "jack o' clubs",
            sf::Style::Default,
            sf::ContextSettings(24, 0, 4, 4, 0, sf::ContextSettings::Attribute::Default, false)),
    _shaderProgram("resources/vertex-shader.glsl", "resources/fragment-shader.glsl")
{
    sf::Vector2u windowSize = _window.getSize();
    sf::ContextSettings windowSettings = _window.getSettings();

    // This code centers the window on the screen
    sf::VideoMode defaultVideoMode = sf::VideoMode::getDesktopMode();
    _window.setPosition(sf::Vector2i(defaultVideoMode.width / 2 - windowSize.x / 2, defaultVideoMode.height / 2 - windowSize.y / 2));
    sf::Vector2i windowPosition = _window.getPosition();

    globalLogger.log("Initialized window:");
    globalLogger.log("    OS Handle:               ", _window.getSystemHandle());
    globalLogger.log("    Size:                    ", windowSize.x, "x", windowSize.y);
    globalLogger.log("    Position:                (", windowPosition.x, ", ", windowPosition.y, ")");
    globalLogger.log("    Depth bits:              ", windowSettings.depthBits);
    globalLogger.log("    Stencil bits:            ", windowSettings.stencilBits);
    globalLogger.log("    Antialiasing level:      ", windowSettings.antialiasingLevel);
    globalLogger.log("    GL version:              ", windowSettings.majorVersion, ".", windowSettings.minorVersion);
    globalLogger.log("    Attribute flags:         ", windowSettings.attributeFlags);
    globalLogger.log("    sRGB capable:            ", windowSettings.sRgbCapable ? "true" : "false");
    globalLogger.log("    Vendor:                  ", glGetString(GL_VENDOR));
    globalLogger.log("    Renderer:                ", glGetString(GL_RENDERER));

    _window.setVerticalSyncEnabled(true);

    glewInit();
    _shaderProgram.compile();

    // Set background color
    glClearDepth(1.f);
    glClearColor(0.f / 256.f, 0.f / 256.f, 0.f / 256.f, 1.f);

    // Make sure things in front get drawn in front
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Don't draw both sides of the face (more efficient)
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);

    // Make sure the perspective matches the window
    setPerspective(windowSize.x, windowSize.y);
}

/*! \callergraph
 *
 * Returns a video mode where the window takes up 1/4 of the screen
 */
sf::VideoMode RenderEngine::getVideoMode() {
    sf::VideoMode defaultVideoMode = sf::VideoMode::getDesktopMode();
    return sf::VideoMode(defaultVideoMode.width / 2, defaultVideoMode.height / 2, defaultVideoMode.bitsPerPixel);
}

/*! \callergraph
 *
 * Defines default values for the other setPerspective
 *
 * \p width  - width of the window     <br>
 * \p height - height of the window    <br>
 */
void RenderEngine::setPerspective(int width, int height) {
    setPerspective(60.f, width, height, 1.f, 10000.f);
}

/*! \callergraph
 *
 * Calls glFustrum with the appropriate values give the input parameters.
 * idk how glFustrum works, but this code is from StackOverflow, so it's probably right.
 *
 * \p fovY   - Desired field of view                          <br>
 * \p width  - Window width                                   <br>
 * \p height - Window height                                  <br>
 * \p zNear  - How close something has to be to be clipped    <br>
 * \p zFar   - How far something has to be to be clipped      <br>
 */
void RenderEngine::setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar) {
    GLdouble aspect = (double) width / height;
    glm::mat4 projection = glm::perspective(glm::radians(fovY), aspect, zNear, zFar);
    _shaderProgram.setMat4("projection", projection);
}

/*! \callergraph
 *
 * Handles notifying OpenGL and SFML's OpenGL that the screen has
 * been resized.
 *
 * \p e - The size event corresponding to this resize (given to us by SFML)
 */
bool RenderEngine::handleResize(const sf::Event::SizeEvent& e) {
    int width = e.width;
    int height = e.height;

    setPerspective(width, height);

    // Update's the window's internal GL states to match the resize
    _window.setView(sf::View(sf::FloatRect(0, 0, width, height)));

    globalLogger.log("Window resized to ", width, "x", height);

    return false;
}

/*! \callergraph
 *
 * Called before doing a full render. Sets OpenGL up to do rendering.
 */
void RenderEngine::beginRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shaderProgram.bind();
}

/*! \callergraph
 *
 * Called after doing a full render. Flushes the buffer to the screen.
 */
void RenderEngine::endRender() {
    _shaderProgram.unbind();
    _window.display();
}

/*! \callergraph
 *
 * Moves the camera based on the player's position.
 *
 * \p position - Player's position
 */
void RenderEngine::translatePlayer(const Player& player) {
    sf::Vector3f position = player.getHeadLocation();
    glm::vec3 eye((float)position.x * SCALE, (float)position.y * SCALE, (float)position.z * SCALE);

    sf::Vector3f rotation = player.getRotation();
    glm::vec3 forward;
    forward.x += sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    forward.y += -sin(glm::radians(rotation.x));
    forward.z += -cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    forward = glm::normalize(forward);

    glm::vec3 worldUp(0.0f,1.0f,0.0f);
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));
    glm::mat4 view = glm::lookAt(eye, eye + forward, up);
    _shaderProgram.setMat4("view", view);
}

/*! \callergraph
 *
 * Renders an axis aligned bounding box. Simply turns off culling
 * and makes rendering use lines instead of fill, then calls
 * pushBlockVertcies() (gotta turn off culling because pushBlockVerticies()
 * is designed so that it renders only the outside of the block,
 * but you can be inside the AABB)
 *
 * \p box   - The AABB to render              <br>
 * \p color - The color to render the AABB    <br>
 */
void RenderEngine::renderAABB(const AABB& box, const sf::Color& color) {
    /*sf::Vector3f p = box.getPosition();
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
    glPopMatrix();*/
}

/*! \callergraph
 *
 * Returns the render window. Useful for making SFML calls.
 */
sf::RenderWindow& RenderEngine::getWindow() {
    return _window;
}
