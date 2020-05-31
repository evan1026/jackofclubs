#pragma once

#include "Rendering/Renderable.h"
#include "Utils/AABB.h"

class AABBRenderer : private Renderable {

    bool _bufInitialized;

    AABBRenderer();

    void pushBlockVertices(std::vector<Vertex>&, const sf::Vector3f& p, const sf::Vector3f& s, sf::Color);

    public:
        static AABBRenderer renderer;

        AABBRenderer(const AABBRenderer& other) = delete;
        AABBRenderer& operator=(const AABBRenderer& other) = delete;

        void renderAABB(RenderEngine& e, const AABB& box, sf::Color color, float lineWidth = 0.1);

        // We want the benefits of Renderable but don't want to actually
        // be rendered so we implement the function to do nothing
        void render(RenderEngine& e, sf::RenderWindow& w) override {}
};
