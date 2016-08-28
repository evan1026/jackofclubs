#include "Graphics/Menu/ColorSelectorMenu.h"

#define SLIDER_WIDTH      30
#define SLIDER_HEIGHT    256
#define START_X           50
#define START_Y          100
#define SPACER            20
#define COLOR_RECT_SIZE  200
#define WIDTH            500
#define HEIGHT           450
#define COLOR_TEXT_SIZE   17
#define TITLE_TEXT_SIZE   30

ColorSelectorMenu::ColorSelectorMenu(sf::Color& color) :
    Menu(sf::Vector2f(WIDTH, HEIGHT), Menu::Type::ColorSelector),
    _redRef(color.r),
    _greenRef(color.g),
    _blueRef(color.b),
    _red(color.r),
    _green(color.g),
    _blue(color.b),
    _redSlider  (0, 255, _red,   sf::Vector2i(START_X, START_Y),
              sf::Vector2i(0,0), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT)),
    _greenSlider(0, 255, _green, sf::Vector2i(START_X + SPACER + SLIDER_WIDTH, START_Y),
              sf::Vector2i(0,0), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT)),
    _blueSlider (0, 255, _blue,  sf::Vector2i(START_X + 2 * SPACER + 2 * SLIDER_WIDTH, START_Y),
              sf::Vector2i(0,0), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT)),
    _colorRect(sf::Vector2f(COLOR_RECT_SIZE, COLOR_RECT_SIZE)),
    _redText  ("Red",   Font::defaultFont),
    _greenText("Green", Font::defaultFont),
    _blueText ("Blue",  Font::defaultFont),
    _titleText("Color Selector", Font::defaultFont)
{
    _redText.setCharacterSize(COLOR_TEXT_SIZE);
    _greenText.setCharacterSize(COLOR_TEXT_SIZE);
    _blueText.setCharacterSize(COLOR_TEXT_SIZE);
    _titleText.setCharacterSize(TITLE_TEXT_SIZE);
}

void ColorSelectorMenu::renderMenu(sf::RenderWindow& w) {
    //Quick cheeky little write-back
    _redRef = _red;
    _greenRef = _green;
    _blueRef = _blue;

    sf::FloatRect bounds = getBounds();
    sf::Vector2i pos(bounds.left, bounds.top);

    _redSlider.setParentPosition(pos);
    _greenSlider.setParentPosition(pos);
    _blueSlider.setParentPosition(pos);

    sf::Vector2i blueSliderPos = _blueSlider.getGlobalPosition();
    _colorRect.setPosition(sf::Vector2f(blueSliderPos.x, blueSliderPos.y)
            + sf::Vector2f(SLIDER_WIDTH + 3 * SPACER, SLIDER_HEIGHT / 2 - COLOR_RECT_SIZE / 2));
    _colorRect.setFillColor(sf::Color(_red, _green, _blue));

    sf::Vector2i redPos = _redSlider.getGlobalPosition();
    sf::Vector2i greenPos = _greenSlider.getGlobalPosition();
    sf::Vector2i bluePos = _blueSlider.getGlobalPosition();

    _redText.setPosition  (redPos.x   + SLIDER_WIDTH / 2 - Utils::textWidth(_redText)   / 2, redPos.y   - 25);
    _greenText.setPosition(greenPos.x + SLIDER_WIDTH / 2 - Utils::textWidth(_greenText) / 2, greenPos.y - 25);
    _blueText.setPosition (bluePos.x  + SLIDER_WIDTH / 2 - Utils::textWidth(_blueText)  / 2, bluePos.y  - 25);
    _titleText.setPosition(pos.x + (float)bounds.width / 2 - Utils::textWidth(_titleText) / 2, pos.y + 25);

    _redSlider.render(w);
    _greenSlider.render(w);
    _blueSlider.render(w);

    w.draw(_colorRect);
    w.draw(_redText);
    w.draw(_greenText);
    w.draw(_blueText);
    w.draw(_titleText);
}

bool ColorSelectorMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    if (getBounds().contains(e.x, e.y)) {
        if (_redSlider.handleMouseButtonPressed(e))   return true;
        if (_greenSlider.handleMouseButtonPressed(e)) return true;
        if (_blueSlider.handleMouseButtonPressed(e))  return true;
        return true;
    }
    return false;
}

bool ColorSelectorMenu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {

    if (_redSlider.handleMouseButtonReleased(e))   return true;
    if (_greenSlider.handleMouseButtonReleased(e)) return true;
    if (_blueSlider.handleMouseButtonReleased(e))  return true;

    if (getBounds().contains(e.x, e.y)) {
        return true;
    }
    return false;
}

bool ColorSelectorMenu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

bool ColorSelectorMenu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    if (getBounds().contains(e.x, e.y)) {
        if (_redSlider.handleMouseMoved(e))   return true;
        if (_greenSlider.handleMouseMoved(e)) return true;
        if (_blueSlider.handleMouseMoved(e))  return true;
        return true;
    }
    return false;
}
