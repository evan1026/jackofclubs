#include "Graphics/Menu/EscapeMenu.h"

#include "Logger/GlobalLogger.hpp"

using Logger::globalLogger;

#define BUTTON_CALLBACK std::bind(&EscapeMenu::buttonCallback, this, std::placeholders::_1)

const static std::string QUIT_BUTTON_STRING = "quitButton";
const static std::string RESUME_BUTTON_STRING = "resumeButton";
const static int BUTTON_WIDTH = 250;
const static int BUTTON_HEIGHT = 75;
const static int BUTTON_PADDING = 20;

/*! \callergraph
 *
 * \p width  - Width of the screen                                                          <br>
 * \p height - Height of the screen                                                         <br>
 * \p g      - Reference to the Game object so that the quit button can call Game::end()    <br>
 */
EscapeMenu::EscapeMenu(int width, int height, Game& g) :
    Menu(sf::Vector2f(width, height), Menu::Type::Escape),
    _game(g),
    _quitButton(sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(BUTTON_WIDTH, BUTTON_HEIGHT), BUTTON_CALLBACK, QUIT_BUTTON_STRING, "Quit"),
    _resumeButton(sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(BUTTON_WIDTH, BUTTON_HEIGHT), BUTTON_CALLBACK, RESUME_BUTTON_STRING, "Resume"),
    _resumeButtonClicked(false)
    {}

/*! \callergraph */
void EscapeMenu::renderMenu(sf::RenderWindow& w) {
    sf::Vector2i quitSize = _quitButton.getSize();
    sf::Vector2i resumeSize = _resumeButton.getSize();
    auto pos = getPosition();
    auto size = getSize();
    int fullHeight = quitSize.y + resumeSize.y + BUTTON_PADDING;

    _resumeButton.setParentPosition(sf::Vector2i(pos.x, pos.y));
    _resumeButton.setLocalPosition (sf::Vector2i(size.x / 2 - resumeSize.x / 2,
                                                 size.y / 2 - fullHeight / 2));

    _quitButton.setParentPosition(sf::Vector2i(pos.x, pos.y));
    _quitButton.setLocalPosition (sf::Vector2i(size.x / 2 - quitSize.x / 2,
                                               size.y / 2 - fullHeight / 2 + resumeSize.y + BUTTON_PADDING));
    _quitButton.render(w);
    _resumeButton.render(w);
}

/*! \callergraph
 *
 * Callback for the buttons. Handles dispatching what they do
 *
 * \p s - The name of the button
 */
void EscapeMenu::buttonCallback(const std::string& s) {
    globalLogger.log("Pressed ", s);

    if (s == QUIT_BUTTON_STRING) {
       _game.end();
    } else if (s == RESUME_BUTTON_STRING) {
        _resumeButtonClicked = true;
    } else {
        globalLogger.error("Unknown button clicked: ", s);
    }

}

/*! \callergraph */
bool EscapeMenu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

/*! \callergraph */
bool EscapeMenu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    return (_quitButton.handleMouseMoved(e) || _resumeButton.handleMouseMoved(e));
}

/*! \callergraph */
bool EscapeMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    _quitButton.handleMouseButtonPressed(e) || _resumeButton.handleMouseButtonPressed(e);
    return !_resumeButtonClicked;
}

/*! \callergraph */
bool EscapeMenu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
    return (_quitButton.handleMouseButtonReleased(e) || _resumeButton.handleMouseButtonReleased(e));
}

/*! \callergraph */
bool EscapeMenu::handleResize(const sf::Event::SizeEvent& e) {
    setSize(sf::Vector2f(e.width, e.height));
    return false;
}
