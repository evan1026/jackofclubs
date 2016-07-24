#include <SFML/System.hpp>

#include "World.h"
#include "Chunk.h"
#include "RenderEngine.h"
#include "NullptrException.h"

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
