#include <algorithm>

#include "Graphics/Components/VBox.h"

VBox::VBox(std::initializer_list<std::shared_ptr<Component>> args) {
    for (auto arg : args) {
        add(arg);
    }
}

void VBox::layout(const sf::RenderWindow& w) {
    Component::layout(w);

    setLocalPosition(0,0);

    int maxWidth = -1;
    int totalY = 0;
    for (auto& child : getChildren()) {
        maxWidth = std::max(maxWidth, child->getSize().x);
        totalY += child->getSize().y;
    }

    setSize(sf::Vector2i(maxWidth, totalY));

    int currY = 0;
    for (auto& child : getChildren()) {
        int xVal = maxWidth / 2.0 - child->getSize().x / 2.0;
        child->setLocalPosition(sf::Vector2i(xVal, currY));
        currY += child->getSize().y;
    }
}
