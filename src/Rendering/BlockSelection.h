#ifndef BLOCK_SELECTION_H
#define BLOCK_SELECTION_H

#include <SFML/System.hpp>
#include <optional>

#include "Rendering/Renderable.h"
#include "World/BlockFace.h"

class BlockSelection : public Renderable {
    std::optional<BlockFace> _selection;
    bool _changed;

    void generateBuffer();
    void pushBlockVertices(std::vector<Vertex>&, const sf::Vector3f& p, const sf::Vector3f& s);

    public:
        BlockSelection();
        void render(RenderEngine& e, sf::RenderWindow& w) override;

        void setSelection(const std::optional<BlockFace>& selection);
        inline std::optional<BlockFace> getSelection() const { return _selection; }
};

#endif
