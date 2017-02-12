#include "Graphics/Menu/{NAME}Menu.h"

#define WIDTH  500
#define HEIGHT 500

{NAME}Menu::{NAME}Menu() : Menu(sf::Vector2f(WIDTH, HEIGHT), Menu::Type::{NAME}) {}

void {NAME}Menu::renderMenu(sf::RenderWindow& w) {
    return;
}

bool {NAME}Menu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

bool {NAME}Menu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    return false;
}

bool {NAME}Menu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    return false;
}

bool {NAME}Menu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return false;
}

bool {NAME}Menu::handleResize(const sf::Event::SizeEvent& e) {
    return false;
}
