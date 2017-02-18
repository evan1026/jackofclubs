#include "Graphics/Menu/EscapeMenu.h"

#include "Logger/GlobalLogger.hpp"

using Logger::globalLogger;

#define OUTTER_PADDING 20

EscapeMenu::EscapeMenu(int width, int height, Game& g) :
    Menu(sf::Vector2f(width - 2 * OUTTER_PADDING, height - 2 * OUTTER_PADDING), Menu::Type::Escape),
    _game(g),
    _quitButton(sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(300, 100), std::bind(&EscapeMenu::testButtonFunction, this, std::placeholders::_1), "quitButton", "Quit")
    {}

void EscapeMenu::renderMenu(sf::RenderWindow& w) {
    _quitButton.setParentPosition(sf::Vector2i(getBounds().left, getBounds().top));
    _quitButton.setLocalPosition (sf::Vector2i(getBounds().width  / 2 - _quitButton.getSize().x / 2,
                                               getBounds().height / 2 - _quitButton.getSize().y / 2));
    _quitButton.render(w);
}

void EscapeMenu::testButtonFunction(const std::string& s) {
    globalLogger.log("Pressed ", s);
    _game.end();
}

bool EscapeMenu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

bool EscapeMenu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    return _quitButton.handleMouseMoved(e);
}

bool EscapeMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    return _quitButton.handleMouseButtonPressed(e);
}

bool EscapeMenu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return _quitButton.handleMouseButtonReleased(e);
}

bool EscapeMenu::handleResize(const sf::Event::SizeEvent& e) {
    setSize(sf::Vector2f(e.width - 2 * OUTTER_PADDING, e.height - 2 * OUTTER_PADDING));
    return false;
}
