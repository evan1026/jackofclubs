#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "Exception/OutOfRangeException.h"
#include "World/Block.h"
#include "World/Chunk.h"
#include "World/World.h"

constexpr float COLOR_SCALE = 256.f / Chunk::BLOCK_COUNT;

/*! \callergraph
 * Need a default constructor for stl containers
 */
Chunk::Chunk() {}

/*! \callergraph
 *
 * Creates a chunk and initializes the blocks to be solid or air based on
 * the y value. Block color starts at black at the (0,0,0) position in the
 * chunk and goes to white at (15,15,15). One color increases in each direction
 * (x is red, y is green, z is blue).
 *
 * \p p     - The position of the chunk in chunk coords (chunk coords == block coords / 16)     <br>
 * \p world - reference to the world (gotta be a pointer because of the default constructor)    <br>
 */
Chunk::Chunk(const sf::Vector3i& p, World* world) : _position(p), _changed(true), _world(world) {
    for (int x = 0; x < BLOCK_COUNT; ++x) {
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int z = 0; z < BLOCK_COUNT; ++z) {
                float blockX = x + _position.x * BLOCK_COUNT;
                float blockY = y + _position.y * BLOCK_COUNT;
                float blockZ = z + _position.z * BLOCK_COUNT;

                Block::Type type = blockY > world->getMaxPosition().y / 2 ? Block::Type::AIR : Block::Type::SOLID;

                _blocks.at(x).at(y).at(z) = Block(sf::Vector3i(blockX, blockY, blockZ),
                                                  sf::Color(x * COLOR_SCALE, y * COLOR_SCALE, z * COLOR_SCALE),
                                                  type);
            }
        }
    }
}

/*! \callergraph
 *
 * Rebuilds the vertex array for this chunk. Works by going through each
 * block (including blocks that are 1 block outside of this chunk), finding
 * the ones that are air, and creating a face at the border of that block
 * and all blocks that are solid and in this chunk. This way, we only render
 * faces if you can actually see them.
 */
void Chunk::rebuildVertArray() {
    _vertArray.clear();

    // Go through each block, including one block ouside
    for (int x = -1; x <= BLOCK_COUNT; ++x) {
        for (int y = -1; y <= BLOCK_COUNT; ++y) {
            for (int z = -1; z <= BLOCK_COUNT; ++z) {
                sf::Vector3i globalPos = localToGlobalBlockPos(sf::Vector3i(x,y,z));

                // If it's not air, who cares, move on
                if (_world->getBlockType(globalPos) != Block::Type::AIR)
                    continue;

                // Ok so we're at an air block, let's make faces around this block
                // if it's in the chunk and we're at the border between solid and
                // air

                // Do it for x+1
                sf::Vector3i target(globalPos.x + 1, globalPos.y, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, -1, getBlock(target).getColor(), sf::Vector2i(1,2));
                }

                // Do it for x-1
                target = sf::Vector3i(globalPos.x - 1, globalPos.y, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, 0, getBlock(target).getColor(), sf::Vector2i(2,1));
                }

                // Do it for y+1
                target = sf::Vector3i(globalPos.x, globalPos.y + 1, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, -1, getBlock(target).getColor(), sf::Vector2i(2,0));
                }

                // Do it for y-1
                target = sf::Vector3i(globalPos.x, globalPos.y - 1, globalPos.z);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, 1, getBlock(target).getColor(), sf::Vector2i(0,2));
                }

                // Do it for z+1
                target = sf::Vector3i(globalPos.x, globalPos.y, globalPos.z + 1);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, -1, getBlock(target).getColor(), sf::Vector2i(0,1));
                }

                // Do it for z-1
                target = sf::Vector3i(globalPos.x, globalPos.y, globalPos.z - 1);
                if (isInChunk(target) && getBlockType(target) == Block::Type::SOLID) {
                    addFace(target, 2, getBlock(target).getColor(), sf::Vector2i(1,0));
                }
            }
        }
    }

    _changed = false;
}

/*! \callergraph
 *
 * Ok, so I'll admit this function looks really weird.
 * The basic idea of it is to automate adding a face to
 * make rebuildVertArray() not be a thousand lines long.
 * It works, I promise. If you want more description, look
 * at the comment below.
 *
 * \p target    - Block we're adding a face to                                                                                        <br>
 * \p addTarget - The dimension to add 1 to initially (used to do faces that don't contain the point at the block's (0,0,0) point)    <br>
 * \p c         - Color of the block                                                                                                  <br>
 * \p order     - Order of dimensions to do move in. Used to make sure the face is always drawn clockwise.                            <br>
 */
void Chunk::addFace(const sf::Vector3i& target, const int& addTarget, const sf::Color& c, const sf::Vector2i& order) {
    float fc[3];
    float fp[3];
    float fn[3];
    static constexpr int oneblock = RenderEngine::SCALE;

    fp[0] = target.x * oneblock; fp[1] = target.y * oneblock; fp[2] = target.z * oneblock;
    if (addTarget != -1) fp[addTarget] += oneblock;

    fc[0] = c.r / 256.f; fc[1] = c.g / 256.f; fc[2] = c.b / 256.f;

    // Okay so this seems a little weird but I can explain
    // So... order contains the order to go around by indexing the dimensions
    // So if the order is (1,0) then we first move in the y, then in the x, then
    // back in the y, and that constitutes the face.
    // The normal needs to point away from the face, so it will point in whatever
    // direction we didn't move in at all. But then you have to also make sure it
    // points in the right direction along that axis, so we also need to handle the
    // last direction.... as for how it works, just remember that the faces are clockwise
    // and think it through...... it works I promise
    fn[order.x] = 0;
    fn[order.y] = 0;

    if (order.x - order.y == 2) {
        fn[3 - order.x - order.y] = 1;
    } else if (order.x - order.y == -2) {
        fn[3 - order.x - order.y] = -1;
    } else {
        fn[3 - order.x - order.y] = order.x - order.y;
    }

    _vertArray.push_back(Vertex(fp,fc,fn));
    fp[order.x] = fp[order.x] + RenderEngine::SCALE;
    _vertArray.push_back(Vertex(fp,fc,fn));
    fp[order.y] = fp[order.y] + RenderEngine::SCALE;
    _vertArray.push_back(Vertex(fp,fc,fn));
    fp[order.x] = fp[order.x] - RenderEngine::SCALE;
    _vertArray.push_back(Vertex(fp,fc,fn));
}

/*! \callergraph
 *
 * Rebuilds the vertex array if needed and then sends it out to OpenGL
 *
 * \p e - Render engine (for OpenGL calls)    <br>
 * \p w - Window (for SFML calls)             <br>
 */
void Chunk::render(RenderEngine& e, sf::RenderWindow& w) {
    if (_changed) {
        rebuildVertArray();
    }
    e.renderVertexArray(_vertArray);
}

/*! \callergraph
 *
 * Converts a global block position to its local position within this chunk
 *
 * \p worldPos - The global position to convert
 */
sf::Vector3i Chunk::globalToLocalBlockPos(const sf::Vector3i& worldPos) const {
    int x = worldPos.x % BLOCK_COUNT;
    int y = worldPos.y % BLOCK_COUNT;
    int z = worldPos.z % BLOCK_COUNT;

    if (x < 0) x += BLOCK_COUNT;
    if (y < 0) y += BLOCK_COUNT;
    if (z < 0) z += BLOCK_COUNT;

    return sf::Vector3i(x, y, z);
}

/*! \callergraph
 *
 * Converts a local position within this chunk to a global position
 *
 * \p localPos - The local position to convert
 */
sf::Vector3i Chunk::localToGlobalBlockPos(const sf::Vector3i& localPos) const {
    return sf::Vector3i(localPos.x + BLOCK_COUNT * _position.x,
                        localPos.y + BLOCK_COUNT * _position.y,
                        localPos.z + BLOCK_COUNT * _position.z);
}

/*! \callergraph
 *
 * Returns a constant reference to the block with the given global position
 *
 * \p worldPos - global position
 */
const Block& Chunk::getBlock(const sf::Vector3i& worldPos) const {
    // To avoid code duplication, we just remove the const, get the block, and
    // then put the const back on what we return
    return const_cast<Chunk*>(this)->getBlock(worldPos);
}

/*! \callergraph
 *
 * Returns a reference to the block at a given global position.
 *
 * \p worldPos - global position
 */
Block& Chunk::getBlock(const sf::Vector3i& worldPos) {
    if (!isInChunk(worldPos)) {
        throw OutOfRangeException();
    }

    sf::Vector3i pos = globalToLocalBlockPos(worldPos);
    return _blocks.at(pos.x).at(pos.y).at(pos.z);
}

/*! \callergraph
 *
 * Returns the type of the block at the given position.
 *
 * \p worldPos - global position
 */
Block::Type Chunk::getBlockType(const sf::Vector3i& worldPos) const {
    return getBlock(worldPos).getType();
}

/*! \callergraph
 *
 * Returns whether or not a block is in this chunk
 *
 * \p pos - global position for the block
 */
bool Chunk::isInChunk(const sf::Vector3i& pos) const {
    return (   pos.x >= _position.x * BLOCK_COUNT && pos.x < _position.x * BLOCK_COUNT + BLOCK_COUNT
            && pos.y >= _position.y * BLOCK_COUNT && pos.y < _position.y * BLOCK_COUNT + BLOCK_COUNT
            && pos.z >= _position.z * BLOCK_COUNT && pos.z < _position.z * BLOCK_COUNT + BLOCK_COUNT);
}

/*! \callergraph
 *
 * Notifys the chunk that it should rebuild its vertex array
 */
void Chunk::notifyChanged() {
    _changed = true;
}
