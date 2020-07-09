#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Rendering/RenderEngine.h"
#include "Rendering/Renderable.h"

/*!
 * Base class for a menu component. Things that would extend this would be buttons,
 * sliders, etc. Anything that is a single item that can be used in multiple places.
 * Handles calculating the component's global position based on its parent's position
 * and its position relative to the parent. Also restricts rendering to work only
 * with an sf::Window (as opposed to also including the render engine, like Renderable).
 * This just makes orthogonal rendering easier.
 */
class Component : public Renderable {

    private:
        /*! The box that encompasses the component */
        sf::Rect<int> _boundingBox;
        /*! The position of the component relative to its parent */
        sf::Vector2i _localPos;

        std::vector<std::shared_ptr<Component>> _children;

        void setGlobalPosition(const sf::Vector2i& pos);
        void setParentPosition(const sf::Vector2i& pos);

    protected:
        virtual void renderComponent(sf::RenderWindow& w) = 0;

    public:
        Component();
        Component(const sf::Vector2i& localPos, const sf::Vector2i& size);
        explicit Component(const sf::Vector2i& size);

        virtual ~Component() = default;

        /*! \callergraph
         *
         * Abstract function that renders the component to the screen
         *
         * \p w - sf::RenderWindow to render to
         */
        virtual void render(RenderEngine&, sf::RenderWindow& w) override;

        void setSize(const sf::Vector2i& size);
        void setLocalPosition(const sf::Vector2i& pos);

        const sf::Vector2i getSize() const;
        const sf::Vector2i getLocalPosition() const;
        const sf::Vector2i getGlobalPosition() const;
        const sf::Vector2i getParentPosition() const;

        const sf::Rect<int> getBounds() const;

        void add(std::shared_ptr<Component>);
        std::shared_ptr<Component> remove(std::shared_ptr<Component>);
};
