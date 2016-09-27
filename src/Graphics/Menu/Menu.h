#ifndef MENU_H
#define MENU_H

#include <memory>

#include "Rendering/IRenderable.h"

/*
 * Base class for all menus. Handles bounds checking and centering
 * on the screen and defines an interface for the derived classes.
 * Also keeps track of the previous menu to allow for menu chaining
 * (e.g. opening a settings menu which opens a graphics settings menu
 * which opens a resolution menu, and you can go backwards through the
 * whole thing when done with the current menu).
 */

class Menu : public IRenderable {

    public:
        // Somewhat restrictive, but useful enough that it makes up for it
        enum class Type {
            ColorSelector
        };

    private:
        sf::RectangleShape _box;
        Type _type;

        void center(sf::RenderWindow& w);

    protected:
        // All derived classes should override this.
        // No gl rendering allowed (just makes things easier);
        virtual void renderMenu(sf::RenderWindow& w) = 0;

        sf::FloatRect getBounds();

    public:
        std::unique_ptr<Menu> prevMenu = nullptr;

        Menu(sf::Vector2f size, Type type);

        void render(RenderEngine& e, sf::RenderWindow& w) override;

        virtual bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) = 0;
        virtual bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) = 0;
        virtual bool handleKeyPressed(const sf::Event::KeyEvent& e) = 0;
        virtual bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) = 0;

        Type getType() { return _type; }
};

#endif
