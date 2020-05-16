#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "RenderEngine.h"

/*!
 * Defines a rendering function so there is a common interface for rendered objects.
 */
class Renderable {

    bool _initialized;
    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _bufferSize;

    protected:
        void initBuffer();
        void setBufferData(const std::vector<Vertex>& vertexes);
        void drawFromBuffer(GLenum type);
        inline unsigned int getVao() const { return _vao; }
        inline unsigned int getVbo() const { return _vbo; }

    public:
        /*! \callergraph
         *
         * Base function for items that need to be rendered
         */
        virtual void render(RenderEngine&, sf::RenderWindow&) = 0;
};

#endif
