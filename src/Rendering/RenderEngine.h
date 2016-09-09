#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#include "Rendering/Vertex.h"
#include "Utils/AABB.h"

class RenderEngine {

    sf::RenderWindow _window;

    static float lightPos[4];
    static float lightAmbient[4];
    static float lightDiffuse[4];

    void pushBlockVertices(const sf::Vector3f& p, const sf::Vector3f& s);
    sf::VideoMode getVideoMode();

    public:
        RenderEngine();

        void setPerspective(int width, int height);
        void setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar);

        void handleResize(int width, int height);

        void beginRender();
        void endRender();

        void renderVertexArray(const std::vector<Vertex>& vertices);
        void renderAABB(const AABB& box, const sf::Color& color);
        void renderBlockSelection(const AABB& box, const sf::Color& color);

        sf::RenderWindow& getWindow();

        void translatePlayer(const sf::Vector3f& position);
        void rotatePlayer(const sf::Vector3f& rotation);
};

#endif
