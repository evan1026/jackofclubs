#pragma once

#include <memory>

#include "Graphics/Components/Component.h"
#include "Utils/Events/IKeyboardEventHandler.h"
#include "Utils/Events/IMouseEventHandler.h"
#include "Utils/Events/IResizeEventHandler.h"

/*!
 * Base class for all menus. Handles bounds checking and centering
 * on the screen and defines an interface for the derived classes.
 * Also keeps track of the previous menu to allow for menu chaining
 * (e.g. opening a settings menu which opens a graphics settings menu
 * which opens a resolution menu, and you can go backwards through the
 * whole thing when done with the current menu).
 */
class Menu : public Component {

    public:
        // Somewhat restrictive, but useful enough that it makes up for it
        /*!
         * All of the types of menus that exist
         */
        enum class Type {
            ColorSelector,
            Escape,
            Settings
        };

    private:
        sf::RectangleShape _box;
        Type _type;

        void center(sf::RenderWindow& w);

    public:
        /*! Pointer to next menu for menu chaining */
        std::unique_ptr<Menu> prevMenu = nullptr;

        Menu(sf::Vector2f size, Type type);
        virtual ~Menu() = default;

        void renderComponent(sf::RenderWindow& w) override;

        /*! \callergraph
         * Returns what type of Menu this one is
         */
        Type getType() { return _type; }
};
