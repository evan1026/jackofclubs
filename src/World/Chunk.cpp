#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "Exception/OutOfRangeException.h"
#include "World/Block.h"
#include "World/Chunk.h"
#include "World/World.h"

constexpr float COLOR_SCALE = 256.f / Chunk::BLOCK_COUNT;

Chunk::Chunk() {}

Chunk::Chunk(const sf::Vector3i& p, World* world) : _position(p), _changed(true), _world(world) {
    for (int x = 0; x < BLOCK_COUNT; ++x) {
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int z = 0; z < BLOCK_COUNT; ++z) {
                float blockX = x + _position.x * BLOCK_COUNT;
                float blockY = y + _position.y * BLOCK_COUNT;
                float blockZ = z + _position.z * BLOCK_COUNT;

                Block::Type type = (-8 * blockY + blockZ * blockX > 0
                                 || -8 * blockY - blockZ * blockX > 0
                                 || 8 * (-128 + blockY) - blockZ * blockX > 0
                                 || 8 * (-128 + blockY) + blockZ * blockX > 0) ? Block::Type::AIR : Block::Type::SOLID;

                _blocks.at(x).at(y).at(z) = Block(sf::Vector3i(blockX, blockY, blockZ),
                                                  sf::Color(x * COLOR_SCALE, y * COLOR_SCALE, z * COLOR_SCALE),
                                                  type);
            }
        }
    }
}

void Chunk::rebuildVertArray() {
    _vertArray.clear();

    for (int x = -1; x <= BLOCK_COUNT; ++x) {
        for (int y = -1; y <= BLOCK_COUNT; ++y) {
            for (int z = -1; z <= BLOCK_COUNT; ++z) {
                sf::Vector3i globalPos = localToGlobalBlockPos(sf::Vector3i(x,y,z));
                if (_world->getBlockType(globalPos) != Block::Type::AIR)
                    continue;

                sf::Vector3i target(globalPos.x + 1, globalPos.y, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, -1, getBlock(target).getColor(), sf::Vector2i(2,1));
                }

                target = sf::Vector3i(globalPos.x - 1, globalPos.y, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, 0, getBlock(target).getColor(), sf::Vector2i(1,2));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y + 1, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, -1, getBlock(target).getColor(), sf::Vector2i(2,0));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y - 1, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, 1, getBlock(target).getColor(), sf::Vector2i(0,2));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y, globalPos.z + 1);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, -1, getBlock(target).getColor(), sf::Vector2i(0,1));
                }

                target = sf::Vector3i(globalPos.x, globalPos.y, globalPos.z - 1);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, 2, getBlock(target).getColor(), sf::Vector2i(1,0));
                }
            }
        }
    }

    _changed = false;
}

void Chunk::addFace(const sf::Vector3i& target, const int& addTarget, const sf::Color& c, const sf::Vector2i& order) {
    float fc[3];
    float fp[3];
    static constexpr int oneblock = Block::SIZE;

    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock;
    if (addTarget != -1) fp[addTarget] += oneblock;

    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;

    _vertArray.push_back(Vertex(fp,fc));
    fp[order.x] = fp[order.x] + Block::SIZE;
    _vertArray.push_back(Vertex(fp,fc));
    fp[order.y] = fp[order.y] + Block::SIZE;
    _vertArray.push_back(Vertex(fp,fc));
    fp[order.x] = fp[order.x] - Block::SIZE;
    _vertArray.push_back(Vertex(fp,fc));
}

void Chunk::render(RenderEngine& e) {
    if (_changed) {
        rebuildVertArray();
    }
    e.renderVertexArray(_vertArray);
}


sf::Vector3i Chunk::globalToLocalBlockPos(const sf::Vector3i& worldPos) const {
    int x = worldPos.x % BLOCK_COUNT;
    int y = worldPos.y % BLOCK_COUNT;
    int z = worldPos.z % BLOCK_COUNT;

    if (x < 0) x += BLOCK_COUNT;
    if (y < 0) y += BLOCK_COUNT;
    if (z < 0) z += BLOCK_COUNT;

    return sf::Vector3i(x, y, z);
}

sf::Vector3i Chunk::localToGlobalBlockPos(const sf::Vector3i& localPos) const {
    return sf::Vector3i(localPos.x + BLOCK_COUNT * _position.x,
                        localPos.y + BLOCK_COUNT * _position.y,
                        localPos.z + BLOCK_COUNT * _position.z);
}

const Block& Chunk::getBlock(const sf::Vector3i& worldPos) const {
    //To avoid code duplication, we just remove the cast, get the block, and
    //then put the const back on what we return
    return const_cast<Chunk*>(this)->getBlock(worldPos);
}

Block& Chunk::getBlock(const sf::Vector3i& worldPos) {
    if (!isInChunk(worldPos)) {
        throw OutOfRangeException();
    }

    sf::Vector3i pos = globalToLocalBlockPos(worldPos);
    return _blocks.at(pos.x).at(pos.y).at(pos.z);
}

Block::Type Chunk::getBlockType(const sf::Vector3i& worldPos) const {
    if (isInChunk(worldPos)) {
        return getBlock(worldPos).getType();
    } else {
        return Block::Type::AIR;
    }
}

bool Chunk::isInChunk(const sf::Vector3i& pos) const {
    return (   pos.x >= _position.x * BLOCK_COUNT && pos.x < _position.x * BLOCK_COUNT + BLOCK_COUNT
            && pos.y >= _position.y * BLOCK_COUNT && pos.y < _position.y * BLOCK_COUNT + BLOCK_COUNT
            && pos.z >= _position.z * BLOCK_COUNT && pos.z < _position.z * BLOCK_COUNT + BLOCK_COUNT);
}

void Chunk::notifyChanged() {
    _changed = true;
}
