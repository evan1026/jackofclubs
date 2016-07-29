#include <SFML/System.hpp>

#include "Block.h"
#include "Chunk.h"
#include "OutOfRangeException.h"
#include "World.h"

constexpr float COLOR_SCALE = 256.f / Chunk::BLOCK_COUNT;

Chunk::Chunk() {}

Chunk::Chunk(const sf::Vector3i& p) : position(p), changed(true) {
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

void Chunk::rebuildVertArray() {
    vertArray.clear();
    World& world = World::getInst();
    int oneblock = Block::SIZE;
    float fp[3];
    float fc[3];

    for (int x = -1; x <= BLOCK_COUNT; ++x) {
        for (int y = -1; y <= BLOCK_COUNT; ++y) {
            for (int z = -1; z <= BLOCK_COUNT; ++z) {
                sf::Vector3i globalPos = localToGlobalBlockPos(sf::Vector3i(x,y,z));
                if (world.getBlockType(globalPos) != Block::Type::AIR)
                    continue;

                sf::Vector3i target(globalPos.x + 1, globalPos.y, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    Block& b = getBlock(target);
                    sf::Color c = b.getColor();
                    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;
                    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock;
                    addFace(fp, fc, sf::Vector2i(2,1));
                }

                target = sf::Vector3i(globalPos.x - 1, globalPos.y, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    Block& b = getBlock(target);
                    sf::Color c = b.getColor();
                    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;
                    fp[0] = target.x * oneblock + oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock;
                    addFace(fp, fc, sf::Vector2i(1,2));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y + 1, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    Block& b = getBlock(target);
                    sf::Color c = b.getColor();
                    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;
                    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock;
                    addFace(fp, fc, sf::Vector2i(2,0));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y - 1, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    Block& b = getBlock(target);
                    sf::Color c = b.getColor();
                    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;
                    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock + oneblock; fp[2] = target.z * oneblock;
                    addFace(fp, fc, sf::Vector2i(0,2));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y, globalPos.z + 1);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    Block& b = getBlock(target);
                    sf::Color c = b.getColor();
                    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;
                    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock;
                    addFace(fp, fc, sf::Vector2i(0,1));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y, globalPos.z - 1);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    Block& b = getBlock(target);
                    sf::Color c = b.getColor();
                    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;
                    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock + oneblock;
                    addFace(fp, fc, sf::Vector2i(1,0));
                }
            }
        }
    }

    changed = false;
}

void Chunk::addFace(float fp[3], float fc[3], const sf::Vector2i& order) {
    vertArray.push_back(Vertex(fp,fc));
    fp[order.x] = fp[order.x] + Block::SIZE;
    vertArray.push_back(Vertex(fp,fc));
    fp[order.y] = fp[order.y] + Block::SIZE;
    vertArray.push_back(Vertex(fp,fc));
    fp[order.x] = fp[order.x] - Block::SIZE;
    vertArray.push_back(Vertex(fp,fc));
}

void Chunk::render(RenderEngine& e) {
    if (changed) {
        rebuildVertArray();
    }

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertArray[0].pos[0]);
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertArray[0].color[0]);

    glDrawArrays(GL_QUADS, 0, vertArray.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}


sf::Vector3i Chunk::globalToLocalBlockPos(const sf::Vector3i& worldPos) {
    int x = worldPos.x % BLOCK_COUNT;
    int y = worldPos.y % BLOCK_COUNT;
    int z = worldPos.z % BLOCK_COUNT;

    if (x < 0) x += BLOCK_COUNT;
    if (y < 0) y += BLOCK_COUNT;
    if (z < 0) z += BLOCK_COUNT;

    return sf::Vector3i(x, y, z);
}

sf::Vector3i Chunk::localToGlobalBlockPos(const sf::Vector3i& localPos) {
    return sf::Vector3i(localPos.x + BLOCK_COUNT * position.x,
                        localPos.y + BLOCK_COUNT * position.y,
                        localPos.z + BLOCK_COUNT * position.z);
}

Block& Chunk::getBlock(const sf::Vector3i& worldPos) {
    if (!isInChunk(worldPos)) {
        throw OutOfRangeException();
    }

    sf::Vector3i pos = globalToLocalBlockPos(worldPos);
    return blocks.at(pos.x).at(pos.y).at(pos.z);
}

Block::Type Chunk::getBlockType(const sf::Vector3i& worldPos) {
    if (isInChunk(worldPos)) {
        return getBlock(worldPos).getType();
    } else {
        return Block::Type::AIR;
    }
}

bool Chunk::isInChunk(const sf::Vector3i& pos) {
    return (pos.x >= position.x * BLOCK_COUNT && pos.x < position.x * BLOCK_COUNT + BLOCK_COUNT
            && pos.y >= position.y * BLOCK_COUNT && pos.y < position.y * BLOCK_COUNT + BLOCK_COUNT
            && pos.z >= position.z * BLOCK_COUNT && pos.z < position.z * BLOCK_COUNT + BLOCK_COUNT);
}

void Chunk::notifyChanged() {
    changed = true;
}
