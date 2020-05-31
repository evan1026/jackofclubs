#pragma once

#include <SFML/Graphics.hpp>

/*!
 * Base class for a menu component. Things that would extend this would be buttons,
 * sliders, etc. Anything that is a single item that can be used in multiple places.
 * Handles calculating the component's global position based on its parent's position
 * and its position relative to the parent. Also restricts rendering to work only
 * with an sf::Window (as opposed to also including the render engine, like Renderable).
 * This just makes orthogonal rendering easier.
 */
class Component {

    protected:
        /*! The box that encompasses the component */
        sf::Rect<int> _boundingBox;
        /*! The position of the component relative to its parent */
        sf::Vector2i _localPos;

        void setGlobalPosition(const sf::Vector2i& pos);

    public:
        Component(const sf::Vector2i& localPos, const sf::Vector2i& parentPos, const sf::Vector2i& size);

        /*! \callergraph
         *
         * Abstract function that renders the component to the screen
         *
         * \p w - sf::RenderWindow to render to
         */
        virtual void render(sf::RenderWindow& w) = 0;

        void setSize(const sf::Vector2i& size);
        void setLocalPosition(const sf::Vector2i& pos);
        void setParentPosition(const sf::Vector2i& pos);

        sf::Vector2i getSize();
        sf::Vector2i getLocalPosition();
        sf::Vector2i getGlobalPosition();
        sf::Vector2i getParentPosition();

        sf::Rect<int> getBounds();
};
