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

/*! \callergraph
 *
 * Constructs a new ColorSelectorMenu by telling it what color variable it should
 * be modifying. Also initializes all of the GUI members.
 *
 * \p color - the color data we will be modifying
 */
ColorSelectorMenu::ColorSelectorMenu(sf::Color& color) :
    Menu(sf::Vector2f(WIDTH, HEIGHT), Menu::Type::ColorSelector),
    _redRef(color.r),   // These three keep references to the original data
    _greenRef(color.g),
    _blueRef(color.b),
    _redSlider  (std::make_shared<Slider<sf::Uint8>>(0, 255, color.r, sf::Vector2i(START_X, START_Y), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT))),
    _greenSlider(std::make_shared<Slider<sf::Uint8>>(0, 255, color.g, sf::Vector2i(START_X + SPACER + SLIDER_WIDTH, START_Y), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT))),
    _blueSlider (std::make_shared<Slider<sf::Uint8>>(0, 255, color.b, sf::Vector2i(START_X + 2 * SPACER + 2 * SLIDER_WIDTH, START_Y), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT))),
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

    add(_redSlider);
    add(_greenSlider);
    add(_blueSlider);
}

/*! \callergraph
 *
 * Recalculates the position of the children and draws them to the screen.
 * Note that this function is called from Menu::render(), which handles
 * the rest of the drawing, so we only need to call w.draw() on all of
 * our components (Slider::render() does this automatically)
 *
 * \p w - the window to draw to
 */
void ColorSelectorMenu::renderComponent(sf::RenderWindow& w) {
    Menu::renderComponent(w);

    // Get our position...
    sf::IntRect bounds = getBounds();
    sf::Vector2i pos(bounds.left, bounds.top);

    {
        // Now grab the last slider's position and make the color preview
        // be to the right of it (3 SPACER's away)
        sf::Vector2i blueSliderPos = _blueSlider->getGlobalPosition();
        _colorRect.setPosition(sf::Vector2f(blueSliderPos.x, blueSliderPos.y)
                + sf::Vector2f(SLIDER_WIDTH + 3 * SPACER, SLIDER_HEIGHT / 2 - COLOR_RECT_SIZE / 2));
        _colorRect.setFillColor(sf::Color(_redRef, _greenRef, _blueRef)); // Gotta make sure the preview has the right color
    }

    {
        // Now we grab the slider positions...
        sf::Vector2i redPos = _redSlider->getGlobalPosition();
        sf::Vector2i greenPos = _greenSlider->getGlobalPosition();
        sf::Vector2i bluePos = _blueSlider->getGlobalPosition();

        // ...and center the text boxes under them
        _redText.setPosition  (redPos.x   + SLIDER_WIDTH / 2 - Utils::textWidth(_redText)   / 2, redPos.y   - 25);
        _greenText.setPosition(greenPos.x + SLIDER_WIDTH / 2 - Utils::textWidth(_greenText) / 2, greenPos.y - 25);
        _blueText.setPosition (bluePos.x  + SLIDER_WIDTH / 2 - Utils::textWidth(_blueText)  / 2, bluePos.y  - 25);
    }

    // Last calculation: center the title text at the top
    _titleText.setPosition(pos.x + (float)bounds.width / 2 - Utils::textWidth(_titleText) / 2, pos.y + 25);

    // And draw the rest of the components
    w.draw(_colorRect);
    w.draw(_redText);
    w.draw(_greenText);
    w.draw(_blueText);
    w.draw(_titleText);
}
