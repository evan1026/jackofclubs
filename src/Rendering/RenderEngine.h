#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#include "Rendering/Vertex.h"
#include "Utils/AABB.h"

/*
 * Handles interfacing with OpenGL. All OpenGL calls must be made through this class.
 */

class RenderEngine {

    sf::RenderWindow _window;

    static float lightPos[4];
    static float lightAmbient[4];
    static float lightDiffuse[4];

    // Given a top left front point and a size, push
    // all of the points in a block to OpenGL
    void pushBlockVertices(const sf::Vector3f& p, const sf::Vector3f& s);

    // Video mode contains things like height, width, depth buffer size, etc.
    sf::VideoMode getVideoMode();

    public:
        RenderEngine();

        // These define how a vertex's position gets translated into a pixel position
        void setPerspective(int width, int height);
        void setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar);

        // Called when the window is resized
        void handleResize(int width, int height);

        // Called before and after a full screen render is done
        void beginRender();
        void endRender();

        // Various functions to render specific things
        void renderVertexArray(const std::vector<Vertex>& vertices);
        void renderAABB(const AABB& box, const sf::Color& color);
        void renderBlockSelection(const AABB& box, const sf::Color& color);

        sf::RenderWindow& getWindow();

        // Moves and rotates the camera based on player position/rotation
        void translatePlayer(const sf::Vector3f& position);
        void rotatePlayer(const sf::Vector3f& rotation);
};

#endif
