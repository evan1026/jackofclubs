#ifndef MENU_H
#define MENU_H

#include <memory>

#include "Rendering/IRenderable.h"

class Menu : public IRenderable {

    public:
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
