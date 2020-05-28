#include "Rendering/BlockSelection.h"
#include "Rendering/AABBRenderer.h"

BlockSelection::BlockSelection() : _selection(std::optional<BlockFace>()) {}

void BlockSelection::render(RenderEngine& e, sf::RenderWindow& w) {
    if (_selection) {
        AABBRenderer::renderer.renderAABB(e, AABB(_selection->getPosition(), sf::Vector3f(1, 1, 1)), sf::Color::White);
    }
}

