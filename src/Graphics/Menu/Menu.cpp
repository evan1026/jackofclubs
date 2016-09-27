#include "Graphics/Menu/Menu.h"

/*
 * Base constructor that sets up the menu background
 * (just a blackish box with a full black outline)
 *
 * size - the height and width of the menu (centered automatically)
 * type - the Menu::Type of the derived class (see Menu.h)
 */
Menu::Menu(sf::Vector2f size, Type type) : _box(size), _type(type) {
    _box.setFillColor(sf::Color(0,0,0,128)); //Black but kinda see-through
    _box.setOutlineColor(sf::Color::Black);
    _box.setOutlineThickness(5);
}

/*
 * Centers the menu on the window
 *
 * w - the window to center on
 */
void Menu::center(sf::RenderWindow& w) {
    sf::Vector2u wsize = w.getSize();
    sf::Vector2f bsize = _box.getSize();
    _box.setPosition(wsize.x / 2 - bsize.x / 2, wsize.y / 2 - bsize.y / 2);
}

/*
 * Renders the window. First it sets itself up to use sfml rendering,
 * then it renders the backround and calls the child class's rendering
 * function. This overrides IRenderable::render.
 *
 * e - the rendering engine for opengl calls (not used)
 * w - the window for sfml calls (used)
 */
void Menu::render(RenderEngine& e, sf::RenderWindow& w) {
    center(w);
    w.pushGLStates();
    w.draw(_box);
    renderMenu(w);
    w.popGLStates();
}

/*
 * Returns a rectangle that represents the menu's position
 * and size on the screen.
 */
sf::FloatRect Menu::getBounds() {
    sf::FloatRect rect = _box.getLocalBounds();
    sf::Vector2f pos = _box.getPosition();
    rect.left = pos.x;
    rect.top = pos.y;
    return rect;
}
