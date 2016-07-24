#ifndef CHUNK_H
#define CHUNK_H

#include <array>

#include "Block.h"
#include "IRenderable.h"
#include "RenderEngine.h"

#define BLOCK_COUNT_DEF 16

typedef std::array<Block, BLOCK_COUNT_DEF>   __line;
typedef std::array<__line, BLOCK_COUNT_DEF>  __plane;
typedef std::array<__plane, BLOCK_COUNT_DEF> __chunk;

class Chunk : public IRenderable {

    __chunk  blocks;
    sf::Vector3i position;

    public:

        static constexpr int BLOCK_COUNT = BLOCK_COUNT_DEF;
        static constexpr float SIZE = BLOCK_COUNT * Block::SIZE;

        Chunk(const sf::Vector3i& position);
        Chunk();

        void render(RenderEngine& e) const;
};

#endif
