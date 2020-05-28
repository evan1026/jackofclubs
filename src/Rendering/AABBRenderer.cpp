#include "Rendering/AABBRenderer.h"

AABBRenderer AABBRenderer::renderer{};

AABBRenderer::AABBRenderer() : _bufInitialized(false) {}

void AABBRenderer::renderAABB(RenderEngine& e, const AABB& box, sf::Color color, float lineWidth) {
    if (!_bufInitialized) {
        initBuffer();
        _bufInitialized = true;
    }

    e.useNoLightingShader();

    sf::Vector3f p = box.getPosition();
    sf::Vector3f s = box.getSize();
    sf::Vector3f linePos;
    sf::Vector3f lineSize;

    // Scale to GL coords
    s = s * RenderEngine::SCALE;
    p = p * RenderEngine::SCALE;

    // Now, we go through the points on the block and render the edges that haven't been taken care of
    // Each block is one point, whereas each call to pushBlockVertices is an edge that ends at that point
    std::vector<Vertex> vertexes;
    linePos = p - sf::Vector3f(lineWidth / 2, lineWidth / 2, lineWidth / 2);
    lineSize = sf::Vector3f(lineWidth, lineWidth, s.z + lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(lineWidth, s.y + lineWidth, lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(s.x + lineWidth, lineWidth, lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);

    linePos = sf::Vector3f(p.x, p.y, p.z + s.z) - sf::Vector3f(lineWidth / 2, lineWidth / 2, -lineWidth / 2);
    lineSize = sf::Vector3f(lineWidth, s.y + lineWidth, -lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(s.x + lineWidth, lineWidth, -lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);

    linePos = sf::Vector3f(p.x, p.y + s.y, p.z) - sf::Vector3f(lineWidth / 2, -lineWidth / 2, lineWidth / 2);
    lineSize = sf::Vector3f(lineWidth, -lineWidth, s.z + lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(s.x + lineWidth, -lineWidth, lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);

    linePos = sf::Vector3f(p.x + s.x, p.y, p.z) - sf::Vector3f(-lineWidth / 2, lineWidth / 2, lineWidth / 2);
    lineSize = sf::Vector3f(-lineWidth, s.y + lineWidth, lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(-lineWidth, lineWidth, s.z + lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);

    linePos = sf::Vector3f(p.x + s.x, p.y + s.y, p.z + s.z)
        - sf::Vector3f(-lineWidth / 2, -lineWidth / 2, -lineWidth / 2);
    lineSize = sf::Vector3f(-lineWidth, -lineWidth, -s.z - lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(-lineWidth, -s.y - lineWidth, -lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);
    lineSize = sf::Vector3f(-s.x - lineWidth, -lineWidth, -lineWidth);
    pushBlockVertices(vertexes, linePos, lineSize, color);

    setBufferData(vertexes);
    drawFromBuffer(GL_TRIANGLES);
}

void AABBRenderer::pushBlockVertices(std::vector<Vertex>& vertexes, const sf::Vector3f& p, const sf::Vector3f& s, sf::Color outlineColor) {
    sf::Vector3f position = p;
    sf::Vector3f size = s;

    if (size.x < 0) {
        position.x += size.x; // Position will get smaller since size is negative
        size.x = -size.x;
    }
    if (size.y < 0) {
        position.y += size.y; // Position will get smaller since size is negative
        size.y = -size.y;
    }
    if (size.z < 0) {
        position.z += size.z; // Position will get smaller since size is negative
        size.z = -size.z;
    }

    float lowX  = position.x,
          highX = position.x + size.x,
          lowY  = position.y,
          highY = position.y + size.y,
          lowZ  = position.z,
          highZ = position.z + size.z;

    float color[] = {outlineColor.r / 256.f, outlineColor.g / 256.f, outlineColor.b / 256.f};

    float posX[] = { 1,  0,  0};
    float negX[] = {-1,  0,  0};
    float posY[] = { 0,  1,  0};
    float negY[] = { 0, -1,  0};
    float posZ[] = { 0,  0,  1};
    float negZ[] = { 0,  0, -1};

    float texCoords[] = {0, 0};

    float points[8][3] = {
        {lowX,  lowY,  lowZ},    // 000
        {lowX,  lowY,  highZ},   // 001
        {lowX,  highY, lowZ},    // 010
        {lowX,  highY, highZ},   // 011
        {highX, lowY,  lowZ},    // 100
        {highX, lowY,  highZ},   // 101
        {highX, highY, lowZ},    // 110
        {highX, highY, highZ}    // 111
    };

    vertexes.emplace_back(points[0b000], color, negY, texCoords);
    vertexes.emplace_back(points[0b001], color, negY, texCoords);
    vertexes.emplace_back(points[0b101], color, negY, texCoords);
    vertexes.emplace_back(points[0b101], color, negY, texCoords);
    vertexes.emplace_back(points[0b100], color, negY, texCoords);
    vertexes.emplace_back(points[0b000], color, negY, texCoords);

    vertexes.emplace_back(points[0b000], color, negZ, texCoords);
    vertexes.emplace_back(points[0b100], color, negZ, texCoords);
    vertexes.emplace_back(points[0b110], color, negZ, texCoords);
    vertexes.emplace_back(points[0b110], color, negZ, texCoords);
    vertexes.emplace_back(points[0b010], color, negZ, texCoords);
    vertexes.emplace_back(points[0b000], color, negZ, texCoords);

    vertexes.emplace_back(points[0b000], color, negX, texCoords);
    vertexes.emplace_back(points[0b010], color, negX, texCoords);
    vertexes.emplace_back(points[0b011], color, negX, texCoords);
    vertexes.emplace_back(points[0b011], color, negX, texCoords);
    vertexes.emplace_back(points[0b001], color, negX, texCoords);
    vertexes.emplace_back(points[0b000], color, negX, texCoords);

    vertexes.emplace_back(points[0b111], color, posY, texCoords);
    vertexes.emplace_back(points[0b011], color, posY, texCoords);
    vertexes.emplace_back(points[0b010], color, posY, texCoords);
    vertexes.emplace_back(points[0b010], color, posY, texCoords);
    vertexes.emplace_back(points[0b110], color, posY, texCoords);
    vertexes.emplace_back(points[0b111], color, posY, texCoords);

    vertexes.emplace_back(points[0b111], color, posZ, texCoords);
    vertexes.emplace_back(points[0b101], color, posZ, texCoords);
    vertexes.emplace_back(points[0b001], color, posZ, texCoords);
    vertexes.emplace_back(points[0b001], color, posZ, texCoords);
    vertexes.emplace_back(points[0b011], color, posZ, texCoords);
    vertexes.emplace_back(points[0b111], color, posZ, texCoords);

    vertexes.emplace_back(points[0b111], color, posX, texCoords);
    vertexes.emplace_back(points[0b110], color, posX, texCoords);
    vertexes.emplace_back(points[0b100], color, posX, texCoords);
    vertexes.emplace_back(points[0b100], color, posX, texCoords);
    vertexes.emplace_back(points[0b101], color, posX, texCoords);
    vertexes.emplace_back(points[0b111], color, posX, texCoords);
}
