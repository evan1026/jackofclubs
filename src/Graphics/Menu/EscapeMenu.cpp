#include <GlobalLogger.hpp>

#include "Graphics/Menu/EscapeMenu.h"
#include "Graphics/Menu/SettingsMenu.h"

using Logger::globalLogger;

#define BUTTON_CALLBACK std::bind(&EscapeMenu::buttonCallback, this, std::placeholders::_1)

const static std::string QUIT_BUTTON_STRING = "quitButton";
const static std::string RESUME_BUTTON_STRING = "resumeButton";
const static std::string SETTINGS_BUTTON_STRING = "settingsButton";
const static int BUTTON_WIDTH = 250;
const static int BUTTON_HEIGHT = 75;
const static int BUTTON_PADDING = 20;

/*! \callergraph
 *
 * \p width  - Width of the screen                                                          <br>
 * \p height - Height of the screen                                                         <br>
 * \p g      - Reference to the Game object so that the quit button can call Game::end()    <br>
 */
EscapeMenu::EscapeMenu(int width, int height, Game& g, Screen& screen) :
    Menu(sf::Vector2f(width, height), Menu::Type::Escape),
    _game(g),
    _screen(screen),
    _quitButton(std::make_shared<Button>(sf::Vector2i(0, 0), sf::Vector2i(BUTTON_WIDTH, BUTTON_HEIGHT), BUTTON_CALLBACK, QUIT_BUTTON_STRING, "Quit")),
    _resumeButton(std::make_shared<Button>(sf::Vector2i(0, 0), sf::Vector2i(BUTTON_WIDTH, BUTTON_HEIGHT), BUTTON_CALLBACK, RESUME_BUTTON_STRING, "Resume")),
    _settingsButton(std::make_shared<Button>(sf::Vector2i(0, 0), sf::Vector2i(BUTTON_WIDTH, BUTTON_HEIGHT), BUTTON_CALLBACK, SETTINGS_BUTTON_STRING, "Settings")),
    _resumeButtonClicked(false)
{
    add(_quitButton);
    add(_resumeButton);
    add(_settingsButton);
}

/*! \callergraph */
void EscapeMenu::renderComponent(sf::RenderWindow& w) {
    Menu::renderComponent(w);

    sf::Vector2i quitSize = _quitButton->getSize();
    sf::Vector2i settingsSize = _settingsButton->getSize();
    sf::Vector2i resumeSize = _resumeButton->getSize();
    auto size = getSize();
    int fullHeight = quitSize.y + resumeSize.y + settingsSize.y + 2 * BUTTON_PADDING;

    _resumeButton->setLocalPosition (sf::Vector2i(size.x / 2 - resumeSize.x / 2,
                                                  size.y / 2 - fullHeight / 2));

    _settingsButton->setLocalPosition (sf::Vector2i(size.x / 2 - settingsSize.x / 2,
                                                    size.y / 2 - fullHeight / 2 + resumeSize.y + BUTTON_PADDING));

    _quitButton->setLocalPosition (sf::Vector2i(size.x / 2 - quitSize.x / 2,
                                                size.y / 2 - fullHeight / 2 + resumeSize.y + settingsSize.y + 2 * BUTTON_PADDING));
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
    } else if (s == SETTINGS_BUTTON_STRING) {
        _screen.addMenu(new SettingsMenu());
    } else {
        globalLogger.error("Unknown button clicked: ", s);
    }

}

/*! \callergraph */
bool EscapeMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    Component::handleMouseButtonPressed(e);
    return !_resumeButtonClicked;
}

/*! \callergraph */
bool EscapeMenu::handleResize(const sf::Event::SizeEvent& e) {
    setSize(sf::Vector2i(e.width, e.height));
    return false;
}
