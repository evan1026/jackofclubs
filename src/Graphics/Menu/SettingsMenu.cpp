#include "Graphics/Menu/SettingsMenu.h"

#define WIDTH  500
#define HEIGHT 500

SettingsMenu::SettingsMenu() : Menu(sf::Vector2f(WIDTH, HEIGHT), Menu::Type::Settings) {}

void SettingsMenu::renderComponent(sf::RenderWindow& w) {
    Menu::renderComponent(w);
    return;
}
