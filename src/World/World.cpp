#include <SFML/System.hpp>

#include "Exception/NullptrException.h"
#include "Exception/OutOfRangeException.h"
#include "Rendering/RenderEngine.h"
#include "World/Chunk.h"
#include "World/World.h"

World* World::inst = nullptr;

World::World() {
    for (int x = -2; x < 2; ++x) {
        for (int y = 0; y < 8; ++y) {
            for (int z = -2; z < 2; ++z) {
                sf::Vector3i pos(x, y, z);
                std::tuple<int, int, int> posTup = std::make_tuple(pos.x, pos.y, pos.z);
                _chunks[posTup] = Chunk(pos);
            }
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
    for (auto i = _chunks.begin(); i != _chunks.end(); ++i) {
        i->second.render(e);
    }
}

std::tuple<int, int, int> World::getChunkPos(const sf::Vector3i& pos) const {
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

const Block& World::getBlock(const sf::Vector3i& pos) const {
    if (!blockExists(pos)) {
        throw OutOfRangeException();
    }

    return _chunks.at(getChunkPos(pos)).getBlock(pos);
}

bool World::blockExists(const sf::Vector3i& pos) const {
    std::tuple<int, int, int> chunkPos = getChunkPos(pos);
    auto iter = _chunks.find(chunkPos);
    return iter != _chunks.end(); //If the block is in a valid chunk, it must exist
}

Block::Type World::getBlockType(const sf::Vector3i& pos) const {
    if (blockExists(pos)) {
        return _chunks.at(getChunkPos(pos)).getBlockType(pos);
    } else {
        return Block::Type::AIR;
    }
}

void World::setBlockType(const sf::Vector3i& pos, const Block::Type& type) {
    if (blockExists(pos)) {
        Chunk& chunk = _chunks.at(getChunkPos(pos));
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
        _chunks.at(getChunkPos(pos)).notifyChanged();
    }
}
