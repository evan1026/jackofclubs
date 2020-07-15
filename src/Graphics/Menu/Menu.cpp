#include "Graphics/Menu/Menu.h"

/*! \callergraph
 *
 * Base constructor that sets up the menu background
 * (just a blackish box with a full black outline)
 *
 * \p size - the height and width of the menu (centered automatically)
 * \p type - the Menu::Type of the derived class (see Menu.h)
 */
Menu::Menu(sf::Vector2f size, Type type) : Component(sf::Vector2i(size)), _box(size), _type(type) {
    _box.setFillColor(sf::Color(0,0,0,128)); //Black but kinda see-through
    _box.setOutlineColor(sf::Color::Black);
    _box.setOutlineThickness(5);
}

/*! \callergraph
 *
 * Centers the menu on the window
 *
 * \p w - the window to center on
 */
void Menu::center(const sf::RenderWindow& w) {
    _box.setSize(sf::Vector2f(getSize()));

    sf::Vector2u wsize = w.getSize();
    sf::Vector2f bsize = _box.getSize();
    _box.setPosition(wsize.x / 2 - bsize.x / 2, wsize.y / 2 - bsize.y / 2);
    setLocalPosition(sf::Vector2i(_box.getPosition()));
}

/*! \callergraph
 *
 * Renders the window. First it sets itself up to use sfml rendering,
 * then it renders the backround and calls the child class's rendering
 * function. This overrides Renderable::render.
 *
 * \p w - the window for sfml calls (used)
 */
void Menu::renderComponent(sf::RenderWindow& w) {
    w.draw(_box);
}

void Menu::layout(const sf::RenderWindow& w) {
    Component::layout(w);
    center(w);
}

bool Menu::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
    // Return value is whether the event has been fully handled, which it has been as long as
    // it's within the bounds of the menu
    Component::handleMouseButtonPressed(e);
    return getBounds().contains(sf::Vector2i(e.x, e.y));
}
