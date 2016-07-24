#ifndef IRENDERABLE_H
#define IRENDERABLE_H

class RenderEngine;

// Interface for renderable objects. Used by RenderEngine.
class IRenderable {
    public:
        virtual void render(RenderEngine&) const = 0;

        void addToEngine();
        void removeFromEngine();
};

#endif
