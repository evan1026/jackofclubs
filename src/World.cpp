#include <SFML/System.hpp>

#include "World.h"
#include "Chunk.h"
#include "RenderEngine.h"
#include "NullptrException.h"
#include "OutOfRangeException.h"

World* World::inst = nullptr;

World::World() {
    for (int i = -2; i < 2; ++i) {
        for (int j = -2; j < 2; ++j) {
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

void World::render(RenderEngine& e) {
    for (auto i = chunks.begin(); i != chunks.end(); ++i) {
        i->second.render(e);
    }
}

std::tuple<int, int, int> World::getChunkPos(const sf::Vector3i& pos) {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    if (pos.x < 0) x += 1;
    if (pos.y < 0) y += 1;
    if (pos.z < 0) z += 1;

    x /= Chunk::BLOCK_COUNT;
    y /= Chunk::BLOCK_COUNT;
    z /= Chunk::BLOCK_COUNT;

    if (pos.x < 0) x -= 1;
    if (pos.y < 0) y -= 1;
    if (pos.z < 0) z -= 1;

    return std::make_tuple(x, y, z);
}

const Block& World::getBlock(const sf::Vector3i& pos) {
    if (!blockExists(pos)) {
        throw OutOfRangeException();
    }

    return chunks.at(getChunkPos(pos)).getBlock(pos);
}

bool World::blockExists(const sf::Vector3i& pos) {
    std::tuple<int, int, int> chunkPos = getChunkPos(pos);
    auto iter = chunks.find(chunkPos);

    return iter != chunks.end(); //If the block is in a valid chunk, it must exist
}

Block::Type World::getBlockType(const sf::Vector3i& pos) {
    if (blockExists(pos)) {
        return chunks.at(getChunkPos(pos)).getBlockType(pos);
    } else {
        return Block::Type::AIR;
    }
}

void World::setBlockType(const sf::Vector3i& pos, const Block::Type& type) {
    if (blockExists(pos)) {
        auto& chunk = chunks.at(getChunkPos(pos));
        chunk.getBlock(pos).setType(type);
        notifyChanged(pos);

        //To avoid render bug where you can see through the world,
        //we notify in all directions to make sure neighboring chunks
        //get updated as well
        notifyChanged(sf::Vector3i(pos.x + 1, pos.y, pos.z));
        notifyChanged(sf::Vector3i(pos.x - 1, pos.y, pos.z));
        notifyChanged(sf::Vector3i(pos.x, pos.y + 1, pos.z));
        notifyChanged(sf::Vector3i(pos.x, pos.y - 1, pos.z));
        notifyChanged(sf::Vector3i(pos.x, pos.y, pos.z + 1));
        notifyChanged(sf::Vector3i(pos.x, pos.y, pos.z - 1));
    }
}

void World::notifyChanged(const sf::Vector3i& pos) {
    if (blockExists(pos)) {
        chunks.at(getChunkPos(pos)).notifyChanged();
    }
}
