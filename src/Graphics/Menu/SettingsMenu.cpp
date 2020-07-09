#include "Graphics/Menu/SettingsMenu.h"

#define WIDTH  500
#define HEIGHT 500

SettingsMenu::SettingsMenu() : Menu(sf::Vector2f(WIDTH, HEIGHT), Menu::Type::Settings) {}

void SettingsMenu::renderComponent(sf::RenderWindow& w) {
    Menu::renderComponent(w);
    return;
}

bool SettingsMenu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

bool SettingsMenu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    return false;
}

bool SettingsMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    return false;
}

bool SettingsMenu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return false;
}

bool SettingsMenu::handleResize(const sf::Event::SizeEvent& e) {
    return false;
}
