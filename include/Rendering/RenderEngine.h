#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <GL/glew.h>
#include "glm/glm.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#include "ShaderProgram.h"
#include "Rendering/Vertex.h"
#include "Utils/AABB.h"
#include "Utils/Events/IResizeEventHandler.h"

class Player;

/*!
 * Handles interfacing with OpenGL. All OpenGL calls must be made through this class.
 */
class RenderEngine : public IResizeEventHandler {

    sf::RenderWindow _window;
    ShaderProgram _shaderProgramLight;
    ShaderProgram _shaderProgramNoLight;

    static glm::vec3 lightPos;
    static glm::vec3 light2Pos;
    static glm::vec4 lightAmbient;
    static glm::vec4 lightDiffuse;
 
    // Given a top left front point and a size, push
    // all of the points in a block to OpenGL
    void pushBlockVertices(const sf::Vector3f& p, const sf::Vector3f& s);

    // Video mode contains things like height, width, depth buffer size, etc.
    sf::VideoMode getVideoMode();

    public:

        /*! Defines the scale between world-coords and gl-coords. If I don't scale it up clipping gets weird */
        static constexpr float SCALE = 5;

        RenderEngine();

        // These define how a vertex's position gets translated into a pixel position
        void setPerspective(int width, int height);
        void setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar);

        // Called when the window is resized
        bool handleResize(const sf::Event::SizeEvent& e) override;

        // Called before and after a full screen render is done
        void beginRender();
        void endRender();

        sf::RenderWindow& getWindow();

        // Moves and rotates the camera based on player position/rotation
        void translatePlayer(const Player& player);

        void useLightingShader();
        void useNoLightingShader();
};

#endif
