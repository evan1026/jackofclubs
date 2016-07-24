#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <memory>

#include "IRenderable.h"

class RenderEngine {

    static RenderEngine* inst;

    std::unordered_set<IRenderable*> renderables; //Needs to be pointers (not references) because of hashing

    RenderEngine();

    public:
    std::shared_ptr<sf::Window> window;
        static void init();
        static void end();
        static RenderEngine& getInst();

        void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
        void addRenderable(IRenderable&);
        void removeRenderable(IRenderable&);

        void render();

        void startRender(const sf::Vector3f& position);
        void color(const sf::Color&);
        void vertex(const sf::Vector3f&);
        void endRender();
};

#endif
