#include <SFML/System.hpp>

#include "Block.h"
#include "Chunk.h"

constexpr float COLOR_SCALE = 256.f / Chunk::BLOCK_COUNT;

Chunk::Chunk() {}

Chunk::Chunk(const sf::Vector3i& p) : position(p) {
    for (int x = 0; x < BLOCK_COUNT; ++x) {
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int z = 0; z < BLOCK_COUNT; ++z) {
                float blockX = x + position.x * BLOCK_COUNT;
                float blockY = y + position.y * BLOCK_COUNT;
                float blockZ = z + position.z * BLOCK_COUNT;
                blocks.at(x).at(y).at(z) = Block(sf::Vector3i(blockX, blockY, blockZ),
                                                 sf::Color(x * COLOR_SCALE, y * COLOR_SCALE, z * COLOR_SCALE));
            }
        }
    }
}

void Chunk::render(RenderEngine& e) const {
    for (int x = 0; x < BLOCK_COUNT; ++x) {
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int z = 0; z < BLOCK_COUNT; ++z) {
                if (  x == 0 || x == BLOCK_COUNT - 1
                   || y == 0 || y == BLOCK_COUNT - 1
                   || z == 0 || z == BLOCK_COUNT - 1) {
                        blocks.at(x).at(y).at(z).render(e);
                }
            }
        }
    }
}
