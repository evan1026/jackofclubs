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
glm::vec3 RenderEngine::lightPos =     { -0.1f,  -1.0f,   0.2f};
glm::vec3 RenderEngine::light2Pos =    {  0.1f,  -1.0f,  -0.2f};
glm::vec4 RenderEngine::lightAmbient = {  0.25f,  0.25f,  0.25f, 1.0f };
glm::vec4 RenderEngine::lightDiffuse = {  0.35f,  0.35f,  0.35f, 1.0f };

/*! \callergraph
 *
 * Creates a rendering window and sets up OpenGL
 */
RenderEngine::RenderEngine() :
    _window(getVideoMode(),
            "jack o' clubs",
            sf::Style::Default,
            sf::ContextSettings(24, 0, 16, 4, 0, sf::ContextSettings::Attribute::Default, false)),
    _shaderProgramLight("resources/vertex-shader.glsl", "resources/fragment-shader.glsl"),
    _shaderProgramNoLight("resources/vertex-shader.glsl", "resources/fragment-shader-no-light.glsl")
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
    _shaderProgramLight.compile();
    _shaderProgramNoLight.compile();

    // Set background color
    glClearDepth(1.f);
    glClearColor(135.f / 256.f, 206.f / 256.f, 235.f / 256.f, 1.f);

    // Make sure things in front get drawn in front
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Don't draw both sides of the face (more efficient)
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);

    // Make sure the perspective matches the window
    setPerspective(windowSize.x, windowSize.y);

    _shaderProgramLight.setVec4("ambientLight", lightAmbient);
    _shaderProgramLight.setVec4("diffuseLight", lightDiffuse);
    _shaderProgramLight.setVec3("light1Pos", lightPos);
    _shaderProgramLight.setVec3("light2Pos", light2Pos);
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
    _shaderProgramLight.setMat4("projection", projection);
    _shaderProgramNoLight.setMat4("projection", projection);
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

void RenderEngine::useLightingShader() {
    _shaderProgramLight.bind();
}

void RenderEngine::useNoLightingShader() {
    _shaderProgramNoLight.bind();
}

/*! \callergraph
 *
 * Called before doing a full render. Sets OpenGL up to do rendering.
 */
void RenderEngine::beginRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    useLightingShader();
}

/*! \callergraph
 *
 * Called after doing a full render. Flushes the buffer to the screen.
 */
void RenderEngine::endRender() {
    _shaderProgramLight.unbind(); // Regardless of which is bound, this will do the unbind
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
    glm::vec3 forward(0,0,0);
    forward.x += sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    forward.y += -sin(glm::radians(rotation.x));
    forward.z += -cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    forward = glm::normalize(forward);

    glm::vec3 worldUp(0.0f,1.0f,0.0f);
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));
    glm::mat4 view = glm::lookAt(eye, eye + forward, up);
    _shaderProgramLight.setMat4("view", view);
    _shaderProgramNoLight.setMat4("view", view);
}

/*! \callergraph
 *
 * Returns the render window. Useful for making SFML calls.
 */
sf::RenderWindow& RenderEngine::getWindow() {
    return _window;
}
