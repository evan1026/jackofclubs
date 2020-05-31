#pragma once

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "RenderEngine.h"

/*!
 * Defines a rendering function so there is a common interface for rendered objects.
 */
class Renderable {

    bool _initialized;
    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _bufferSize;

    static std::map<std::string, GLint> _textureIndexes;

    static bool loadTexture(std::string name);

    protected:
        void setTexture(std::string name);
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
