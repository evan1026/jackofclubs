#pragma once

#include <unordered_map>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>

#include "Player.h"
#include "Rendering/Renderable.h"
#include "Utils/tuple_fix.h"
#include "World/Block.h"
#include "World/Chunk.h"

/*!
 * Class for the entire world as a whole. Contains methods for interfacing
 * with the world like changing blocks and getting their state.
 */
class World : public Renderable {

    std::unordered_map<std::tuple<int, int, int>, Chunk> _chunks; //Tuple because I found code to hash it online

    // Returns what chunk a global position would be in
    std::tuple<int, int, int> getChunkPos(const sf::Vector3i& pos) const;

    // Notify chunks at particular global positions that they were changed
    void notifyChangedSingle(const sf::Vector3i& pos);
    void notifyChanged(const sf::Vector3i& pos);

    public:
        World();

        void render(RenderEngine& e, sf::RenderWindow& w) override;

        const Block& getBlock(const sf::Vector3i& pos) const;

        bool blockExists(const sf::Vector3i& pos) const;

        Block::Type getBlockType(const sf::Vector3i& pos) const;
        void setBlockType(const sf::Vector3i& pos, const Block::Type& type);

        sf::Color getBlockColor(const sf::Vector3i& pos) const;
        void setBlockColor(const sf::Vector3i& pos, const sf::Color& color);

        bool checkCollision(const Player& player) const;

        sf::Vector3i getMinPosition() const;
        sf::Vector3i getMaxPosition() const;
};
