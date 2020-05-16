#include "BlockSelection.h"

#define LINE_WIDTH 0.1

BlockSelection::BlockSelection() : _selection(std::optional<BlockFace>()), _changed(false) {
    initBuffer();
}

void BlockSelection::render(RenderEngine& e, sf::RenderWindow& w) {
    if (_changed) {
        generateBuffer();
    }

    e.useNoLightingShader();
    drawFromBuffer(GL_TRIANGLES);
}

void BlockSelection::setSelection(const std::optional<BlockFace>& selection) {
    if (bool(selection) != bool(_selection) || (selection && (selection->getPosition() != _selection->getPosition()))) {
        _changed = true;
    }
    _selection = selection;
}

void BlockSelection::generateBuffer() {
    _changed = false;

    if (!_selection) {
        setBufferData(std::vector<Vertex>());
        return;
    }

    sf::Vector3f p = _selection->getPosition();
    sf::Vector3f s = sf::Vector3f(1, 1, 1);
    sf::Vector3f linePos;
    sf::Vector3f lineSize;

    // Scale to GL coords
    s = s * RenderEngine::SCALE;
    p = p * RenderEngine::SCALE;

    // Now, we go through the points on the block and render the edges that haven't been taken care of
    // Each block is one point, whereas each call to pushBlockVertices is an edge that ends at that point
    std::vector<Vertex> vertexes;
    linePos = p - sf::Vector3f(LINE_WIDTH / 2, LINE_WIDTH / 2, LINE_WIDTH / 2);
    lineSize = sf::Vector3f(LINE_WIDTH, LINE_WIDTH, s.z + LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(LINE_WIDTH, s.y + LINE_WIDTH, LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(s.x + LINE_WIDTH, LINE_WIDTH, LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);

    linePos = sf::Vector3f(p.x, p.y, p.z + RenderEngine::SCALE) - sf::Vector3f(LINE_WIDTH / 2, LINE_WIDTH / 2, -LINE_WIDTH / 2);
    lineSize = sf::Vector3f(LINE_WIDTH, s.y + LINE_WIDTH, -LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(s.x + LINE_WIDTH, LINE_WIDTH, -LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);

    linePos = sf::Vector3f(p.x, p.y + RenderEngine::SCALE, p.z) - sf::Vector3f(LINE_WIDTH / 2, -LINE_WIDTH / 2, LINE_WIDTH / 2);
    lineSize = sf::Vector3f(LINE_WIDTH, -LINE_WIDTH, s.z + LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(s.x + LINE_WIDTH, -LINE_WIDTH, LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);

    linePos = sf::Vector3f(p.x + RenderEngine::SCALE, p.y, p.z) - sf::Vector3f(-LINE_WIDTH / 2, LINE_WIDTH / 2, LINE_WIDTH / 2);
    lineSize = sf::Vector3f(-LINE_WIDTH, s.y + LINE_WIDTH, LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(-LINE_WIDTH, LINE_WIDTH, s.z + LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);

    linePos = sf::Vector3f(p.x + RenderEngine::SCALE, p.y + RenderEngine::SCALE, p.z + RenderEngine::SCALE)
        - sf::Vector3f(-LINE_WIDTH / 2, -LINE_WIDTH / 2, -LINE_WIDTH / 2);
    lineSize = sf::Vector3f(-LINE_WIDTH, -LINE_WIDTH, -s.z - LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(-LINE_WIDTH, -s.y - LINE_WIDTH, -LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);
    lineSize = sf::Vector3f(-s.x - LINE_WIDTH, -LINE_WIDTH, -LINE_WIDTH);
    pushBlockVertices(vertexes, linePos, lineSize);

    setBufferData(vertexes);
}

void BlockSelection::pushBlockVertices(std::vector<Vertex>& vertexes, const sf::Vector3f& p, const sf::Vector3f& s) {
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

    // Default outline color is black
    sf::Color outlineColor = sf::Color::Black;

    // But for darker blocks, make it white to improve visibility
    sf::Color blockColor = _selection->getColor();
    if (blockColor.r < 128 && blockColor.g < 128 && blockColor.b < 128) {
        outlineColor = sf::Color::White;
    }
    float color[] = {outlineColor.r / 256.f, outlineColor.g / 256.f, outlineColor.b / 256.f};

    float posX[] = { 1,  0,  0};
    float negX[] = {-1,  0,  0};
    float posY[] = { 0,  1,  0};
    float negY[] = { 0, -1,  0};
    float posZ[] = { 0,  0,  1};
    float negZ[] = { 0,  0, -1};

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

    vertexes.emplace_back(points[0b000], color, negY);
    vertexes.emplace_back(points[0b001], color, negY);
    vertexes.emplace_back(points[0b101], color, negY);
    vertexes.emplace_back(points[0b101], color, negY);
    vertexes.emplace_back(points[0b100], color, negY);
    vertexes.emplace_back(points[0b000], color, negY);

    vertexes.emplace_back(points[0b000], color, negZ);
    vertexes.emplace_back(points[0b100], color, negZ);
    vertexes.emplace_back(points[0b110], color, negZ);
    vertexes.emplace_back(points[0b110], color, negZ);
    vertexes.emplace_back(points[0b010], color, negZ);
    vertexes.emplace_back(points[0b000], color, negZ);

    vertexes.emplace_back(points[0b000], color, negX);
    vertexes.emplace_back(points[0b010], color, negX);
    vertexes.emplace_back(points[0b011], color, negX);
    vertexes.emplace_back(points[0b011], color, negX);
    vertexes.emplace_back(points[0b001], color, negX);
    vertexes.emplace_back(points[0b000], color, negX);

    vertexes.emplace_back(points[0b111], color, posY);
    vertexes.emplace_back(points[0b011], color, posY);
    vertexes.emplace_back(points[0b010], color, posY);
    vertexes.emplace_back(points[0b010], color, posY);
    vertexes.emplace_back(points[0b110], color, posY);
    vertexes.emplace_back(points[0b111], color, posY);

    vertexes.emplace_back(points[0b111], color, posZ);
    vertexes.emplace_back(points[0b101], color, posZ);
    vertexes.emplace_back(points[0b001], color, posZ);
    vertexes.emplace_back(points[0b001], color, posZ);
    vertexes.emplace_back(points[0b011], color, posZ);
    vertexes.emplace_back(points[0b111], color, posZ);

    vertexes.emplace_back(points[0b111], color, posX);
    vertexes.emplace_back(points[0b110], color, posX);
    vertexes.emplace_back(points[0b100], color, posX);
    vertexes.emplace_back(points[0b100], color, posX);
    vertexes.emplace_back(points[0b101], color, posX);
    vertexes.emplace_back(points[0b111], color, posX);
}
