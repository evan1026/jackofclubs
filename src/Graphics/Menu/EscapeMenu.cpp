#include "Graphics/Menu/EscapeMenu.h"

#define OUTTER_PADDING 20

EscapeMenu::EscapeMenu(int width, int height) :
    Menu(sf::Vector2f(width - 2 * OUTTER_PADDING, height - 2 * OUTTER_PADDING), Menu::Type::Escape) {}

void EscapeMenu::renderMenu(sf::RenderWindow& w) {
    return;
}

bool EscapeMenu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

bool EscapeMenu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    return false;
}

bool EscapeMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    return false;
}

bool EscapeMenu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return false;
}

bool EscapeMenu::handleResize(const sf::Event::SizeEvent& e) {
    setSize(sf::Vector2f(e.width - 2 * OUTTER_PADDING, e.height - 2 * OUTTER_PADDING));
    return false;
}
