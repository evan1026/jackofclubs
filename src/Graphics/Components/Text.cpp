#include "Graphics/Components/Text.h"

Text::Text() : Text(sf::Vector2i(0,0)) {}
Text::Text(sf::Vector2i localPos, const sf::String& text, const sf::Font& font) : Component(localPos, false /*childrenAllowed*/), _theText(text, font) {
    sf::FloatRect textBounds = _theText.getGlobalBounds();
    setSize(sf::Vector2i(textBounds.width, textBounds.height));
}
Text::Text(sf::Vector2i localPos, unsigned int fontSize, const sf::String& text, const sf::Font& font) : Text(localPos, text, font) {
    _theText.setCharacterSize(fontSize);
}

void Text::renderComponent(sf::RenderWindow& w) {
    sf::FloatRect textSize = _theText.getLocalBounds();
    _theText.setOrigin(textSize.left + textSize.width / 2, textSize.top + textSize.height / 2);
    _theText.setPosition(sf::Vector2f(getGlobalPosition()) + sf::Vector2f(textSize.width / 2, textSize.height / 2));
    w.draw(_theText);
}

void Text::layout(const sf::RenderWindow& w) {
    sf::FloatRect textBounds = _theText.getGlobalBounds();
    setSize(sf::Vector2i(textBounds.width, textBounds.height));
}

void Text::setFillColor(const sf::Color& color) {
    _theText.setFillColor(color);
}

const sf::Color& Text::getFillColor() {
    return _theText.getFillColor();
}

unsigned int Text::getFontSize() {
    return _theText.getCharacterSize();
}

const sf::String& Text::getString() {
    return _theText.getString();
}

void Text::setFontSize(unsigned int size) {
    _theText.setCharacterSize(size);
}

void Text::setString(const sf::String& string) {
    _theText.setString(string);
}

float Text::textWidth() {
    sf::Vector2f start = _theText.findCharacterPos(0);
    sf::Vector2f end   = _theText.findCharacterPos(std::numeric_limits<std::size_t>::max());
    return end.x - start.x;
}

float Text::textHeight() {
    return _theText.getLocalBounds().height;
}
