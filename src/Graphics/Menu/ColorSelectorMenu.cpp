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
    _colorRect(std::make_shared<Rectangle>(sf::Vector2i(COLOR_RECT_SIZE, COLOR_RECT_SIZE))),
    _redText  (std::make_shared<Text>(sf::Vector2i(0,0), "Red")),
    _greenText(std::make_shared<Text>(sf::Vector2i(0,0), "Green")),
    _blueText (std::make_shared<Text>(sf::Vector2i(0,0), "Blue")),
    _titleText(std::make_shared<Text>(sf::Vector2i(0,0), "Color Selector"))
{
    _redText->setFontSize(COLOR_TEXT_SIZE);
    _greenText->setFontSize(COLOR_TEXT_SIZE);
    _blueText->setFontSize(COLOR_TEXT_SIZE);
    _titleText->setFontSize(TITLE_TEXT_SIZE);

    add(_redSlider);
    add(_greenSlider);
    add(_blueSlider);
    add(_colorRect);
    add(_redText);
    add(_greenText);
    add(_blueText);
    add(_titleText);
}

void ColorSelectorMenu::layout(const sf::RenderWindow& w) {
    Menu::layout(w);

    // Get our position...
    sf::IntRect bounds = getBounds();

    {
        // Now grab the last slider's position and make the color preview
        // be to the right of it (3 SPACER's away)
        sf::Vector2i blueSliderPos = _blueSlider->getLocalPosition();
        _colorRect->setLocalPosition(blueSliderPos + sf::Vector2i(SLIDER_WIDTH + 3 * SPACER, SLIDER_HEIGHT / 2 - COLOR_RECT_SIZE / 2));
        _colorRect->setFillColor(sf::Color(_redRef, _greenRef, _blueRef)); // Gotta make sure the preview has the right color
    }

    {
        // Now we grab the slider positions...
        sf::Vector2i redPos = _redSlider->getLocalPosition();
        sf::Vector2i greenPos = _greenSlider->getLocalPosition();
        sf::Vector2i bluePos = _blueSlider->getLocalPosition();

        // ...and center the text boxes under them
        _redText->setLocalPosition  (sf::Vector2i(redPos.x   + SLIDER_WIDTH / 2 - _redText->textWidth()   / 2, redPos.y   - 25));
        _greenText->setLocalPosition(sf::Vector2i(greenPos.x + SLIDER_WIDTH / 2 - _greenText->textWidth() / 2, greenPos.y - 25));
        _blueText->setLocalPosition (sf::Vector2i(bluePos.x  + SLIDER_WIDTH / 2 - _blueText->textWidth()  / 2, bluePos.y  - 25));
    }

    // Last calculation: center the title text at the top
    _titleText->setLocalPosition(sf::Vector2i((float)bounds.width / 2 - _titleText->textWidth() / 2, 25));

}
