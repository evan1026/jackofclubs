#include "Graphics/Components/Button.h"
#include "Utils/Font.h"

/*! \callergraph */
bool Button::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    if (getBounds().contains(e.x, e.y)) {
        _rect->setFillColor(sf::Color(200, 200, 200));
    } else {
        _rect->setFillColor(sf::Color::White);
    }
    return false;
}

/*! \callergraph */
bool Button::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    if (getBounds().contains(e.x, e.y)) {
        _function(_name);
        return true;
    }

    return false;
}

void Button::layout(const sf::RenderWindow& w) {
    Component::layout(w);

    sf::Vector2i size = getSize();
    _rect->setSize(size);
    _rect->setLocalPosition(sf::Vector2i(0,0));

    sf::Vector2i textSize = _text->getSize();
    _text->setLocalPosition(sf::Vector2i(size.x / 2.f - textSize.x / 2.f, size.y / 2.f - textSize.y / 2.f));
}
