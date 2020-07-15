#pragma once

#include <functional>

#include "Graphics/Components/Component.h"
#include "Graphics/Components/Rectangle.h"
#include "Graphics/Components/Text.h"
#include "Utils/Events/IMouseEventHandler.h"

using ButtonFunction = std::function<void(const std::string&)>;

/*!
 * When the user clicks on it, it calls the callback function. Pretty simple.
 */
class Button : public Component {

    ButtonFunction _function;
    std::string _name;

    std::shared_ptr<Rectangle> _rect;
    std::shared_ptr<Text> _text;

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
            Component(pos, size, false /*childrenAllowed*/),
            _function(f),
            _name(name),
            _rect(std::make_shared<Rectangle>(sf::Vector2i(0,0), size)),
            _text(std::make_shared<Text>(sf::Vector2i(0,0), text))
            {
                forceAdd(_rect);
                forceAdd(_text);
                _rect->setFillColor(sf::Color::White);
                _text->setFillColor(sf::Color::Black);
            }

        virtual ~Button() = default;

        virtual void layout(const sf::RenderWindow& w) override;

        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;

};
