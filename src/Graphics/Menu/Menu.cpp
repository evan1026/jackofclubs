#include "Graphics/Menu/Menu.h"

Menu::Menu(sf::Vector2f size, Type type) : _box(size), _type(type) {
    _box.setFillColor(sf::Color(0,0,0,128)); //Black by kinda see-through
    _box.setOutlineColor(sf::Color::Black);
    _box.setOutlineThickness(5);
}

void Menu::center(sf::RenderWindow& w) {
    sf::Vector2u wsize = w.getSize();
    sf::Vector2f bsize = _box.getSize();
    _box.setPosition(wsize.x / 2 - bsize.x / 2, wsize.y / 2 - bsize.y / 2);
}

void Menu::render(RenderEngine& e, sf::RenderWindow& w) {
    center(w);
    w.pushGLStates();
    w.draw(_box);
    renderMenu(w);
    w.popGLStates();
}

sf::FloatRect Menu::getBounds() {
    sf::FloatRect rect = _box.getLocalBounds();
    sf::Vector2f pos = _box.getPosition();
    rect.left = pos.x;
    rect.top = pos.y;
    return rect;
}
