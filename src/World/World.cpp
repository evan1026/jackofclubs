#include <cmath>

#include <SFML/System.hpp>

#include "Exception/OutOfRangeException.h"
#include "World/World.h"

#define NUM_CHUNKS_LENGTH 2   // Actually half the total length
#define NUM_CHUNKS_HEIGHT 8


/*! \callergraph
 * Creates a new world which is 4 chunks by 4 chunks by 8 chunks (size is just for testing purposes)
 */
World::World() {
    for (int x = -NUM_CHUNKS_LENGTH; x < NUM_CHUNKS_LENGTH; ++x) {
        for (int y = 0; y < NUM_CHUNKS_HEIGHT; ++y) {
            for (int z = -NUM_CHUNKS_LENGTH; z < NUM_CHUNKS_LENGTH; ++z) {
                sf::Vector3i pos(x, y, z);
                std::tuple<int, int, int> posTup = std::make_tuple(pos.x, pos.y, pos.z);
                _chunks[posTup] = Chunk(pos, this);
            }
        }
    }
}

/*! \callergraph
 *
 * Forwards the rendering on to each chunk.
 *
 * \p e - Render engine (used for OpenGL calls)    <br>
 * \p w - Window (for SFML calls)                  <br>
 */
void World::render(RenderEngine& e, sf::RenderWindow& w) {
    for (auto i = _chunks.begin(); i != _chunks.end(); ++i) {
        i->second.render(e, w);
    }
}

/*! \callergraph
 *
 * Gets the position of the chunk a particular position is in
 *
 * \p pos - global position
 */
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

/*! \callergraph
 *
 * Returns the block at a sepcified position
 *
 * \p pos - global position
 */
const Block& World::getBlock(const sf::Vector3i& pos) const {
    if (!blockExists(pos)) {
        throw OutOfRangeException();
    }

    return _chunks.at(getChunkPos(pos)).getBlock(pos);
}

/*! \callergraph
 *
 * Basically checks if a position is in one of the chunks that actually exist.
 *
 * \p pos - global position
 */
bool World::blockExists(const sf::Vector3i& pos) const {
    std::tuple<int, int, int> chunkPos = getChunkPos(pos);
    auto iter = _chunks.find(chunkPos);
    return iter != _chunks.end(); //If the block is in a valid chunk, it must exist
}

/*! \callergraph
 *
 * Gets the type of a block, if it exists, and returns air if not
 *
 * \p pos - global position
 */
Block::Type World::getBlockType(const sf::Vector3i& pos) const {
    if (blockExists(pos)) {
        return _chunks.at(getChunkPos(pos)).getBlockType(pos);
    } else {
        return Block::Type::AIR;
    }
}

/*! \callergraph
 *
 * Sets the type of a block at a given position
 *
 * \p pos  - global position            <br>
 * \p type - new type for that block    <br>
 */
void World::setBlockType(const sf::Vector3i& pos, const Block::Type& type) {
    if (blockExists(pos)) {
        Chunk& chunk = _chunks.at(getChunkPos(pos));
        chunk.getBlock(pos).setType(type);
        notifyChanged(pos);
    }
}

/*! \callergraph
 *
 * Gets the color of a block at a specific position.
 * Returns black if it doesn't exist
 *
 * \p pos - global position
 */
sf::Color World::getBlockColor(const sf::Vector3i& pos) const {
    if (blockExists(pos)) {
        return _chunks.at(getChunkPos(pos)).getBlock(pos).getColor();
    } else {
        return sf::Color::Black;
    }
}

/*! \callergraph
 *
 * Sets the color of a block
 *
 * \p pos   - global position    <br>
 * \p color - new color          <br>
 */
void World::setBlockColor(const sf::Vector3i& pos, const sf::Color& color) {
    if (blockExists(pos)) {
        Chunk& chunk = _chunks.at(getChunkPos(pos));
        chunk.getBlock(pos).setColor(color);
        notifyChanged(pos);
    }
}

/*! \callergraph
 *
 * Notifys the chunk that contains a changed block that a block has been changed,
 * and also notifies the chunks that touch that block (otherwise changing blocks
 * that are next to a chunk border could cause you to see through the world)
 *
 * \p pos - global position of changed block
 */
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

/*! \callergraph
 *
 * Base function for notifyChanged that doesn't notify the
 * chunks around the given block
 *
 * \p pos - global position of changed block
 */
void World::notifyChangedSingle(const sf::Vector3i& pos) {
    if (blockExists(pos)) {
        _chunks.at(getChunkPos(pos)).notifyChanged();
    }
}

/*! \callergraph
 *
 * Checks if player is colliding with a block
 * Goes through all blocks in a range slightly
 * larger than the player and checks if there's
 * a collision with that block. If no collisions,
 * return false, else true.
 *
 * \p player - The player to check for a collision
 */
bool World::checkCollision(const Player& player) const {
    auto position = player.getPosition();
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

/*! \callergraph
 *
 * Gets the position vector with the smallest possible values for x, y, and z
 */
sf::Vector3i World::getMinPosition() const {
    return sf::Vector3i(-NUM_CHUNKS_LENGTH * Chunk::BLOCK_COUNT, 0, -NUM_CHUNKS_LENGTH * Chunk::BLOCK_COUNT);
}

/*! \callergraph
 *
 * Gets the position vector with the greatest possible values for x, y, and z
 */
sf::Vector3i World::getMaxPosition() const {
    return sf::Vector3i(NUM_CHUNKS_LENGTH * Chunk::BLOCK_COUNT, NUM_CHUNKS_HEIGHT * Chunk::BLOCK_COUNT, NUM_CHUNKS_LENGTH * Chunk::BLOCK_COUNT);
}
