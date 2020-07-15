#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Rendering/RenderEngine.h"
#include "Rendering/Renderable.h"
#include "Utils/Events/IKeyboardEventHandler.h"
#include "Utils/Events/IMouseEventHandler.h"
#include "Utils/Events/IResizeEventHandler.h"

class Component;

template<typename T>
concept BoolFunc = std::is_invocable_r_v<bool, T, Component&>;

/*!
 * Base class for a menu component. Things that would extend this would be buttons,
 * sliders, etc. Anything that is a single item that can be used in multiple places.
 * Handles calculating the component's global position based on its parent's position
 * and its position relative to the parent. Also restricts rendering to work only
 * with an sf::Window (as opposed to also including the render engine, like Renderable).
 * This just makes orthogonal rendering easier.
 */
class Component : public Renderable, public IMouseEventHandler, public IKeyboardEventHandler, public IResizeEventHandler {

    private:
        /*! The box that encompasses the component */
        sf::Rect<int> _boundingBox;
        /*! The position of the component relative to its parent */
        sf::Vector2i _localPos;

        bool _childrenAllowed;

        std::vector<std::shared_ptr<Component>> _children;

        void setGlobalPosition(const sf::Vector2i& pos);
        void setParentPosition(const sf::Vector2i& pos);

        bool delegateToChildren(BoolFunc auto func) {
            bool finished;
            for (auto child : _children) {
                finished = finished || func(*child);
                if (finished) {
                    break;
                }
            }
            return finished;
        }

    protected:
        virtual void renderComponent(sf::RenderWindow& w);

        void forceAdd(std::shared_ptr<Component>);
        std::shared_ptr<Component> forceRemove(std::shared_ptr<Component>);

    public:
        Component(bool children = true);
        Component(const sf::Vector2i& localPos, const sf::Vector2i& size, bool children = true);
        explicit Component(const sf::Vector2i& size, bool children = true);

        virtual ~Component() = default;

        /*! \callergraph
         *
         * Abstract function that renders the component to the screen
         *
         * \p w - sf::RenderWindow to render to
         */
        virtual void render(RenderEngine&, sf::RenderWindow& w) override;
        virtual void layout(const sf::RenderWindow& w);

        void setSize(const sf::Vector2i& size);
        void setLocalPosition(const sf::Vector2i& pos);

        const sf::Vector2i getSize() const;
        const sf::Vector2i getLocalPosition() const;
        const sf::Vector2i getGlobalPosition() const;
        const sf::Vector2i getParentPosition() const;

        const sf::Rect<int> getBounds() const;

        void add(std::shared_ptr<Component>);
        std::shared_ptr<Component> remove(std::shared_ptr<Component>);

        virtual bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        virtual bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;
        virtual bool handleKeyPressed(const sf::Event::KeyEvent& e) override;
        virtual bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        virtual bool handleResize(const sf::Event::SizeEvent& e) override;
};
