#include <SFML/System.hpp>

#include "Exception/NullptrException.h"
#include "Exception/OutOfRangeException.h"
#include "Player.h"
#include "Rendering/RenderEngine.h"
#include "World/Chunk.h"
#include "World/World.h"

World::World() {
    for (int x = -2; x < 2; ++x) {
        for (int y = 0; y < 8; ++y) {
            for (int z = -2; z < 2; ++z) {
                sf::Vector3i pos(x, y, z);
                std::tuple<int, int, int> posTup = std::make_tuple(pos.x, pos.y, pos.z);
                _chunks[posTup] = Chunk(pos, this);
            }
        }
    }
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
    }
}

sf::Color World::getBlockColor(const sf::Vector3i& pos) const {
    if (blockExists(pos)) {
        return _chunks.at(getChunkPos(pos)).getBlock(pos).getColor();
    } else {
        return sf::Color::Black;
    }
}

void World::setBlockColor(const sf::Vector3i& pos, const sf::Color& color) {
    if (blockExists(pos)) {
        Chunk& chunk = _chunks.at(getChunkPos(pos));
        chunk.getBlock(pos).setColor(color);
        notifyChanged(pos);
    }
}

void World::notifyChanged(const sf::Vector3i& pos) {
        notifyChangedSingle(pos);

        //To avoid render bug where you can see through the world,
        //we notify in all directions to make sure neighboring chunks
        //get updated as well
        notifyChangedSingle(sf::Vector3i(pos.x + 1, pos.y, pos.z));
        notifyChangedSingle(sf::Vector3i(pos.x - 1, pos.y, pos.z));
        notifyChangedSingle(sf::Vector3i(pos.x, pos.y + 1, pos.z));
        notifyChangedSingle(sf::Vector3i(pos.x, pos.y - 1, pos.z));
        notifyChangedSingle(sf::Vector3i(pos.x, pos.y, pos.z + 1));
        notifyChangedSingle(sf::Vector3i(pos.x, pos.y, pos.z - 1));
}

void World::notifyChangedSingle(const sf::Vector3i& pos) {
    if (blockExists(pos)) {
        _chunks.at(getChunkPos(pos)).notifyChanged();
    }
}

bool World::checkCollision(const Player& player) const {
    auto& position = player.getPosition();
    std::vector<Block> collision;

    for (int x = std::floor(position.x); x <= std::ceil(position.x + 1); ++x) {

        for (int z = std::floor(position.z); z <= std::ceil(position.z + 1); ++z) {
            sf::Vector3i top, bottom;

            for (int y = std::floor(position.y); y <= std::ceil(position.y + 3); ++y) {
                sf::Vector3i blockPos(x, y, z);

                if (blockExists(blockPos)) {

                    if (player.checkCollision(getBlock(blockPos))) {
                        return true;
                    }

                } // if block exists

            } //for y
        } //for z
    } // for x

    return false;
}
