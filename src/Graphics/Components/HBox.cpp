#include <algorithm>

#include "Graphics/Components/HBox.h"

HBox::HBox(std::initializer_list<std::shared_ptr<Component>> args) {
    for (auto arg : args) {
        add(arg);
    }
}

void HBox::layout(const sf::RenderWindow& w) {
    Component::layout(w);

    setLocalPosition(0,0);

    int maxHeight = -1;
    int totalX = 0;
    for (auto& child : getChildren()) {
        maxHeight = std::max(maxHeight, child->getSize().y);
        totalX += child->getSize().x;
    }

    setSize(sf::Vector2i(totalX, maxHeight));

    int currX = 0;
    for (auto& child : getChildren()) {
        int yVal = maxHeight / 2.0 - child->getSize().y / 2.0;
        child->setLocalPosition(sf::Vector2i(currX, yVal));
        currX += child->getSize().x;
    }
}
