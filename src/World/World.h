#ifndef WORLD_H
#define WORLD_H

#include <SFML/System.hpp>
#include <unordered_map>

#include "Rendering/IRenderable.h"
#include "Utils/tuple_fix.h"
#include "World/Block.h"
#include "World/Chunk.h"

class World : public IRenderable {

    static World* inst;

    std::unordered_map<std::tuple<int, int, int>, Chunk> _chunks; //Tuple because I found code to hash it online

    World();

    std::tuple<int, int, int> getChunkPos(const sf::Vector3i& pos) const;

    public:
        virtual ~World(); //virtual just to silence warnings; doesn't make an actual difference here

        static void init();
        static void end();
        static World& getInst();

        void render(RenderEngine& e);

        const Block& getBlock(const sf::Vector3i& pos) const;
        bool blockExists(const sf::Vector3i& pos) const;
        Block::Type getBlockType(const sf::Vector3i& pos) const;
        void setBlockType(const sf::Vector3i& pos, const Block::Type& type);

        void notifyChanged(const sf::Vector3i& pos);
};

#endif
