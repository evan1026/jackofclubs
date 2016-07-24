#include <SFML/System.hpp>

#include "World.h"
#include "Chunk.h"
#include "RenderEngine.h"
#include "NullptrException.h"
#include "OutOfRangeException.h"

World* World::inst = nullptr;

World::World() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            sf::Vector3i pos(i, 0, j);
            std::tuple<int, int, int> posTup = std::make_tuple(pos.x, pos.y, pos.z);
            chunks[posTup] = Chunk(pos);
        }
    }

    addToEngine();
}

World::~World() {
    removeFromEngine();
}

void World::init() {
    if (inst == nullptr) {
        inst = new World();
    }
}

void World::end() {
    if (inst != nullptr) {
        delete inst;
    }
}

World& World::getInst() {
    if (inst == nullptr) {
        throw NullptrException();
    }
    return *inst;
}

void World::render(RenderEngine& e) const {
    for (auto i = chunks.begin(); i != chunks.end(); ++i) {
        i->second.render(e);
    }
}

std::tuple<int, int, int> World::getChunkPos(const sf::Vector3i& pos) {
    return std::make_tuple(pos.x / Chunk::BLOCK_COUNT, pos.y / Chunk::BLOCK_COUNT, pos.z / Chunk::BLOCK_COUNT);
}

Block& World::getBlock(const sf::Vector3i& pos) {
    if (!blockExists(pos)) {
        throw OutOfRangeException();
    }

    return chunks[getChunkPos(pos)].getBlock(pos);
}

bool World::blockExists(const sf::Vector3i& pos) {
    std::tuple<int, int, int> chunkPos = getChunkPos(pos);
    auto iter = chunks.find(chunkPos);

    if (iter != chunks.end()) {
        return chunks[chunkPos].blockExists(pos);
    } else {
        return false;
    }
}

Block::Type World::getBlockType(const sf::Vector3i& pos) {
    if (blockExists(pos)) {
        return chunks[getChunkPos(pos)].getBlockType(pos);
    } else {
        return Block::Type::AIR;
    }
}
