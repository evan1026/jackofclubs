#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <SFML/System.hpp>

#include "IRenderable.h"
#include "Chunk.h"
#include "tuple_fix.h"

class World : public IRenderable {

    static World* inst;

    std::unordered_map<std::tuple<int, int, int>, Chunk> chunks;

    World();

    public:
        virtual ~World(); //virtual just to silence warnings; doesn't make an actual difference here

        static void init();
        static void end();
        static World& getInst();

        void render(RenderEngine& e) const;
};

#endif
