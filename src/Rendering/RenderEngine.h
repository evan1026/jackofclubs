#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <unordered_set>
#include <vector>

#include "Rendering/IRenderable.h"
#include "Rendering/Vertex.h"

class RenderEngine {

    static RenderEngine* inst;

    std::unordered_set<IRenderable*> _renderables; //Needs to be pointers (not references) because of hashing

    RenderEngine(int width, int height);

    public:
        std::shared_ptr<sf::Window> window; //TODO move after having made control classes

        static void init(int width, int height);
        static void end();
        static RenderEngine& getInst();

        void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
        void addRenderable(IRenderable&);
        void removeRenderable(IRenderable&);

        void render();

        void renderVertexArray(const std::vector<Vertex>& vertices);
};

#endif
