#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <unordered_set>
#include <vector>

#include "Rendering/IRenderable.h"
#include "Rendering/Vertex.h"
#include "Utils/AABB.h"

class RenderEngine {

    sf::Window _window;
    std::unordered_set<IRenderable*> _renderables; //Needs to be pointers (not references) because of hashing

    static RenderEngine* inst;

    public:
        static RenderEngine& getInst();

        RenderEngine(int width, int height);

        void setPerspective(GLdouble fovY, int width, int height, GLdouble zNear, GLdouble zFar);

        void addRenderable(IRenderable&);
        void removeRenderable(IRenderable&);

        void handleResize(int width, int height);

        void render();

        void renderVertexArray(const std::vector<Vertex>& vertices);
        void renderAABB(const AABB& box, const sf::Color& color);

        sf::Window& getWindow();

        void translatePlayer(const sf::Vector3f& position);
        void rotatePlayer(const sf::Vector3f& rotation);
};

#endif
