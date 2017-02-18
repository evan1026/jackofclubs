#ifndef BUTTON_H
#define BUTTON_H

#include <functional>

#include "Graphics/Components/Component.h"
#include "Utils/Events/MouseEventHandler.h"

using ButtonFunction = std::function<void(const std::string&)>;

class Button : public Component, public MouseEventHandler {

    ButtonFunction _function;
    std::string _name;
    std::string _text;
    bool _mouseOver;

    public:
        Button(const sf::Vector2i& pos, const sf::Vector2i& parentPos, const sf::Vector2i& size, ButtonFunction f, const std::string& name, const std::string& text) :
            Component(pos, parentPos, size),
            _function(f),
            _name(name),
            _text(text),
            _mouseOver(false)
            {}

        void render(sf::RenderWindow& w) override;

        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;

};

#endif
