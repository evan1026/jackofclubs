#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <unordered_map>

#include "Player.h"
#include "Rendering/IRenderable.h"
#include "Utils/tuple_fix.h"
#include "World/Block.h"
#include "World/Chunk.h"

class World : public IRenderable {

    std::unordered_map<std::tuple<int, int, int>, Chunk> _chunks; //Tuple because I found code to hash it online

    std::tuple<int, int, int> getChunkPos(const sf::Vector3i& pos) const;

    void notifyChangedSingle(const sf::Vector3i& pos);
    void notifyChanged(const sf::Vector3i& pos);

    public:
        World();
        virtual ~World(); //virtual just to silence warnings; doesn't make an actual difference here

        void render(RenderEngine& e);

        const Block& getBlock(const sf::Vector3i& pos) const;
        bool blockExists(const sf::Vector3i& pos) const;

        Block::Type getBlockType(const sf::Vector3i& pos) const;
        void setBlockType(const sf::Vector3i& pos, const Block::Type& type);
        sf::Color getBlockColor(const sf::Vector3i& pos) const;
        void setBlockColor(const sf::Vector3i& pos, const sf::Color& color);

        bool checkCollision(const Player& player) const;
};

#endif
