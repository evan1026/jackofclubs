#include <SFML/System.hpp>

#include "Block.h"
#include "Chunk.h"
#include "OutOfRangeException.h"

constexpr float COLOR_SCALE = 256.f / Chunk::BLOCK_COUNT;

Chunk::Chunk() {}

Chunk::Chunk(const sf::Vector3i& p) : position(p) {
    for (int x = 0; x < BLOCK_COUNT; ++x) {
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int z = 0; z < BLOCK_COUNT; ++z) {
                float blockX = x + position.x * BLOCK_COUNT;
                float blockY = y + position.y * BLOCK_COUNT;
                float blockZ = z + position.z * BLOCK_COUNT;

                Block::Type type = Block::Type::SOLID;

                blocks.at(x).at(y).at(z) = Block(sf::Vector3i(blockX, blockY, blockZ),
                                                 sf::Color(x * COLOR_SCALE, y * COLOR_SCALE, z * COLOR_SCALE),
                                                 type);
            }
        }
    }
}

void Chunk::render(RenderEngine& e) const {
    e.pushMatrix();
    for (int x = 0; x < BLOCK_COUNT; ++x) {
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int z = 0; z < BLOCK_COUNT; ++z) {
                blocks.at(x).at(y).at(z).render(e);
            }
        }
    }
    e.popMatrix();
}


sf::Vector3i Chunk::translatePos(const sf::Vector3i& worldPos) {
    return sf::Vector3i(worldPos.x % BLOCK_COUNT, worldPos.y % BLOCK_COUNT, worldPos.z % BLOCK_COUNT);
}

Block& Chunk::getBlock(const sf::Vector3i& worldPos) {
    if (!blockExists(worldPos)) {
        throw OutOfRangeException();
    }

    sf::Vector3i pos = translatePos(worldPos);

    return blocks.at(pos.x).at(pos.y).at(pos.z);
}

bool Chunk::blockExists(const sf::Vector3i& worldPos) {
    sf::Vector3i pos = translatePos(worldPos);

    return (!(pos.x >= BLOCK_COUNT || pos.y >= BLOCK_COUNT || pos.z >= BLOCK_COUNT
             || pos.x < 0 || pos.y < 0 || pos.z < 0));
}

Block::Type Chunk::getBlockType(const sf::Vector3i& worldPos) {
    if (blockExists(worldPos)) {
        return getBlock(worldPos).getType();
    } else {
        return Block::Type::AIR;
    }
}
