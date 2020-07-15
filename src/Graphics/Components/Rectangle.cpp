#include "Graphics/Components/Rectangle.h"

void Rectangle::renderComponent(sf::RenderWindow& w) {
    w.draw(_theRectangle);
}

void Rectangle::layout(const sf::RenderWindow& w) {
    _theRectangle.setPosition(sf::Vector2f(getGlobalPosition()));
    _theRectangle.setSize(sf::Vector2f(getSize()));
}

void Rectangle::setFillColor(const sf::Color& color) {
    _theRectangle.setFillColor(color);
}

void Rectangle::setOutlineColor(const sf::Color& color) {
    _theRectangle.setOutlineColor(color);
}

void Rectangle::setOutlineThickness(float thickness) {
    _theRectangle.setOutlineThickness(thickness);
}

const sf::Color& Rectangle::getFillColor() const {
    return _theRectangle.getFillColor();
}

const sf::Color& Rectangle::getOutlineColor() const {
    return _theRectangle.getOutlineColor();
}

float Rectangle::getOutlineThickness() const {
    return _theRectangle.getOutlineThickness();
}
