#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <vector>

#include "Block.h"
#include "RenderEngine.h"
#include "Vertex.h"

#define BLOCK_COUNT_DEF 16

typedef std::array<Block, BLOCK_COUNT_DEF>   __line;
typedef std::array<__line, BLOCK_COUNT_DEF>  __plane;
typedef std::array<__plane, BLOCK_COUNT_DEF> __chunk;

class Chunk {

    __chunk  blocks;
    sf::Vector3i position;
    std::vector<Vertex> vertArray;
    bool changed;

    sf::Vector3i globalToLocalBlockPos(const sf::Vector3i& worldPos);
    sf::Vector3i localToGlobalBlockPos(const sf::Vector3i& localPos);
    void rebuildVertArray();
    void addFace(float fp[3], float fc[3], const sf::Vector2i& order);

    public:

        static constexpr int BLOCK_COUNT = BLOCK_COUNT_DEF;
        static constexpr float SIZE = BLOCK_COUNT * Block::SIZE;

        Chunk(const sf::Vector3i& position);
        Chunk();

        void render(RenderEngine& e);

        Block& getBlock(const sf::Vector3i& pos);
        Block::Type getBlockType(const sf::Vector3i& pos);
        bool isInChunk(const sf::Vector3i& pos);

        void notifyChanged();
};

#endif
