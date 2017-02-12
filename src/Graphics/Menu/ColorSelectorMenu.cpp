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
    _redSlider  (0, 255, _redRef,   sf::Vector2i(START_X, START_Y), sf::Vector2i(0,0), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT)),
    _greenSlider(0, 255, _greenRef, sf::Vector2i(START_X + SPACER + SLIDER_WIDTH, START_Y), sf::Vector2i(0,0), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT)),
    _blueSlider (0, 255, _blueRef,  sf::Vector2i(START_X + 2 * SPACER + 2 * SLIDER_WIDTH, START_Y), sf::Vector2i(0,0), sf::Vector2i(SLIDER_WIDTH, SLIDER_HEIGHT)),
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

/*! \callergraph
 *
 * Recalculates the position of the children and draws them to the screen.
 * Note that this function is called from Menu::render(), which handles
 * the rest of the drawing, so we only need to call w.draw() on all of
 * our components (Slider::render() does this automatically)
 *
 * \p w - the window to draw to
 */
void ColorSelectorMenu::renderMenu(sf::RenderWindow& w) {

    // Get our position...
    sf::FloatRect bounds = getBounds();
    sf::Vector2i pos(bounds.left, bounds.top);

    // ...so we can tell our kids about it
    _redSlider.setParentPosition(pos);
    _greenSlider.setParentPosition(pos);
    _blueSlider.setParentPosition(pos);

    {
        // Now grab the last slider's position and make the color preview
        // be to the right of it (3 SPACER's away)
        sf::Vector2i blueSliderPos = _blueSlider.getGlobalPosition();
        _colorRect.setPosition(sf::Vector2f(blueSliderPos.x, blueSliderPos.y)
                + sf::Vector2f(SLIDER_WIDTH + 3 * SPACER, SLIDER_HEIGHT / 2 - COLOR_RECT_SIZE / 2));
        _colorRect.setFillColor(sf::Color(_redRef, _greenRef, _blueRef)); // Gotta make sure the preview has the right color
    }

    {
        // Now we grab the slider positions...
        sf::Vector2i redPos = _redSlider.getGlobalPosition();
        sf::Vector2i greenPos = _greenSlider.getGlobalPosition();
        sf::Vector2i bluePos = _blueSlider.getGlobalPosition();

        // ...and center the text boxes under them
        _redText.setPosition  (redPos.x   + SLIDER_WIDTH / 2 - Utils::textWidth(_redText)   / 2, redPos.y   - 25);
        _greenText.setPosition(greenPos.x + SLIDER_WIDTH / 2 - Utils::textWidth(_greenText) / 2, greenPos.y - 25);
        _blueText.setPosition (bluePos.x  + SLIDER_WIDTH / 2 - Utils::textWidth(_blueText)  / 2, bluePos.y  - 25);
    }

    // Last calculation: center the title text at the top
    _titleText.setPosition(pos.x + (float)bounds.width / 2 - Utils::textWidth(_titleText) / 2, pos.y + 25);

    // Render the sliders
    _redSlider.render(w);
    _greenSlider.render(w);
    _blueSlider.render(w);

    // And draw the rest of the components
    w.draw(_colorRect);
    w.draw(_redText);
    w.draw(_greenText);
    w.draw(_blueText);
    w.draw(_titleText);
}

/*! \callergraph
 *
 * Handles a mouse click. Just forwards the click to the sliders if they clicked on us.
 * Gotta return true after each forward because if one guy handled it, we don't even need to
 * tell the rest about it.
 *
 * \p e - The click event we're handling
 */
bool ColorSelectorMenu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    if (getBounds().contains(e.x, e.y)) {
        if (_redSlider.handleMouseButtonPressed(e))   return true;
        if (_greenSlider.handleMouseButtonPressed(e)) return true;
        if (_blueSlider.handleMouseButtonPressed(e))  return true;
        return true;
    }
    return false;
}

/*! \callergraph
 *
 * Handles a mouse button release event. Differs from the mouse click in that it doesn't
 * do bounds checking before forwarding to the children. This is so that if they drag outside
 * the bounds of the menu and then release, the children aren't left thinking the mouse movements
 * still apply to them.
 *
 * \p e - The mouse release event we're handling
 */
bool ColorSelectorMenu::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {

    if (_redSlider.handleMouseButtonReleased(e))   return true;
    if (_greenSlider.handleMouseButtonReleased(e)) return true;
    if (_blueSlider.handleMouseButtonReleased(e))  return true;

    if (getBounds().contains(e.x, e.y)) {
        return true;
    }
    return false;
}

/*! \callergraph
 *
 * We don't actually handle key presses (if they press c again, the screen will handle it),
 * but I have it here because I need to override this function from Menu.
 *
 * \p e - The key event we're ignoring
 */
bool ColorSelectorMenu::handleKeyPressed(const sf::Event::KeyEvent& e) {
    return false;
}

/*! \callergraph
 *
 * Forwards mouse movements on to the children if the movement was on this menu.
 * The bounds checking doesn't make too much of a difference, but I think it makes
 * the behavior a bit more intuitive.
 *
 * \p e - The movement event we're handling
 */
bool ColorSelectorMenu::handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
    if (getBounds().contains(e.x, e.y)) {
        if (_redSlider.handleMouseMoved(e))   return true;
        if (_greenSlider.handleMouseMoved(e)) return true;
        if (_blueSlider.handleMouseMoved(e))  return true;
        return true;
    }
    return false;
}

bool ColorSelectorMenu::handleResize(const sf::Event::SizeEvent& e) {
    return false;
}
