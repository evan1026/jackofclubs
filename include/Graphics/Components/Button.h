#pragma once

#include <functional>

#include "Graphics/Components/Component.h"
#include "Utils/Events/IMouseEventHandler.h"

using ButtonFunction = std::function<void(const std::string&)>;

/*!
 * When the user clicks on it, it calls the callback function. Pretty simple.
 */
class Button : public Component, public IMouseEventHandler {

    ButtonFunction _function;
    std::string _name;
    std::string _text;
    bool _mouseOver;

    public:
        /*! \callergraph
         *
         * \p pos       - The position of the button relative to its parent                                                                                                 <br>
         * \p size      - The size of the button                                                                                                                            <br>
         * \p f         - The callback function to call when the button is clicked. Should have the signature @code{.cpp} void callback(const std::string& text) @endcode   <br>
         * \p name      - The name of the button. This string is sent to the callback so that you can have multiple buttons call the same function.                         <br>
         * \p text      - The text to display on the button                                                                                                                 <br>
         */
        Button(const sf::Vector2i& pos, const sf::Vector2i& size, ButtonFunction f, const std::string& name, const std::string& text) :
            Component(pos, size),
            _function(f),
            _name(name),
            _text(text),
            _mouseOver(false)
            {}

        void renderComponent(sf::RenderWindow& w) override;

        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;

};
