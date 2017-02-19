#include "Graphics/Components/Button.h"
#include "Utils/Font.h"

/*! \callergraph */
bool Button::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    _mouseOver = getBounds().contains(e.x, e.y);
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

/*! \callergraph */
bool Button::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return false;
}

/*! \callergraph */
void Button::render(sf::RenderWindow& w) {
    sf::Vector2i size = getSize();
    sf::RectangleShape rect(sf::Vector2f(size.x, size.y));

    if (_mouseOver) {
        rect.setFillColor(sf::Color(200, 200, 200));
    } else {
        rect.setFillColor(sf::Color::White);
    }

    sf::Vector2i pos = getGlobalPosition();
    rect.setPosition(pos.x, pos.y);
    w.draw(rect);


    sf::Text text(_text, Font::defaultFont);
    text.setFillColor(sf::Color::Black);
    sf::FloatRect textSize = text.getGlobalBounds();
    text.setOrigin(textSize.left + textSize.width / 2, textSize.top + textSize.height / 2);
    text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
    w.draw(text);
}
