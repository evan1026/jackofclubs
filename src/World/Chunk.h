#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <vector>

#include "Rendering/IRenderable.h"
#include "Rendering/RenderEngine.h"
#include "Rendering/Vertex.h"
#include "World/Block.h"

#define BLOCK_COUNT_DEF 16

class World;

/*
 * One Chunk is 16x16x16 blocks. Contains the actual
 * blocks and has functions for interfacing with them
 */

typedef std::array<Block, BLOCK_COUNT_DEF>   __line;
typedef std::array<__line, BLOCK_COUNT_DEF>  __plane;
typedef std::array<__plane, BLOCK_COUNT_DEF> __chunk;

class Chunk : public IRenderable {

    __chunk _blocks;
    sf::Vector3i _position;
    std::vector<Vertex> _vertArray;
    bool _changed;
    World* _world; // Points back up to the world just so that we can work with it easier

    sf::Vector3i globalToLocalBlockPos(const sf::Vector3i& worldPos) const;
    sf::Vector3i localToGlobalBlockPos(const sf::Vector3i& localPos) const;

    void rebuildVertArray();
    void addFace(const sf::Vector3i& target, const int& addTarget, const sf::Color& c, const sf::Vector2i& order);

    public:

        static constexpr int BLOCK_COUNT = BLOCK_COUNT_DEF;

        // Full width of this chunk in OpenGL coordinates
        // TODO should this be in render engine?
        static constexpr float RENDER_WIDTH = BLOCK_COUNT * Block::SIZE;

        Chunk(const sf::Vector3i& position, World* world);
        Chunk();

        void render(RenderEngine& e, sf::RenderWindow& w) override;

        Block& getBlock(const sf::Vector3i& pos);
        const Block& getBlock(const sf::Vector3i& pos) const;

        Block::Type getBlockType(const sf::Vector3i& pos) const;

        bool isInChunk(const sf::Vector3i& pos) const;

        // Notifies chunk that it has been changed and should
        // rebuild its vertex array
        void notifyChanged();
};

#endif
