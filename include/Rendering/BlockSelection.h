#pragma once

#include <optional>

#include "Rendering/Renderable.h"
#include "World/BlockFace.h"

class BlockSelection : public Renderable {
    std::optional<BlockFace> _selection;

    public:
        BlockSelection();
        void render(RenderEngine& e, sf::RenderWindow& w) override;

        void setSelection(const std::optional<BlockFace>& selection) { _selection = selection; }
        inline std::optional<BlockFace> getSelection() const { return _selection; }
};
