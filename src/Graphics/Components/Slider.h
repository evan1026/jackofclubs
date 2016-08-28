#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>
#include <type_traits>

#include "Graphics/Components/Component.h"
#include "Utils/Utils.h"
#include "Utils/Font.h"

#define SLIDER_LINE_WIDTH 4
#define SLIDER_BAR_HEIGHT 10

template<typename T>
class Slider : public Component {
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Slider can only be contructed with number types!");

    sf::RectangleShape _sliderLine;
    sf::RectangleShape _sliderBar;
    sf::Text _text;

    T _min;
    T _max;

    T& _value;

    bool _capturedMouse = false;

    public:
        Slider(T min, T max, T& value, const sf::Vector2i& localPos, const sf::Vector2i& parentPos, const sf::Vector2i& size) :
            Component(localPos, parentPos, size),
            _sliderLine(sf::Vector2f(SLIDER_LINE_WIDTH, size.y)),
            _sliderBar(sf::Vector2f(size.x, SLIDER_BAR_HEIGHT)),
            _text(Utils::toString(value), Font::defaultFont),
            _min(min),
            _max(max),
            _value(value)
        {
            _sliderLine.setFillColor(sf::Color(200, 200, 200));
            _sliderBar.setFillColor(sf::Color::White);

            //Next two loops make sure the text is small enough to fit
            int charSize = 30;
            _text.setString(Utils::toString(_max));
            _text.setCharacterSize(charSize);
            while (Utils::textWidth(_text) > _sliderBar.getSize().x) {
                charSize--;
                _text.setCharacterSize(charSize);
            }

            _text.setString(Utils::toString(_min));
            _text.setCharacterSize(charSize);
            while (Utils::textWidth(_text) > _sliderBar.getSize().x) {
                charSize--;
                _text.setCharacterSize(charSize);
            }
        }

        void render(sf::RenderWindow& w) override {
            _text.setString(Utils::toString(_value));

            float barHeight = float(_value - _min) / (_max - _min) * _boundingBox.height;

            _sliderLine.setPosition(sf::Vector2f(_boundingBox.left, _boundingBox.top) + sf::Vector2f(_boundingBox.width / 2 - SLIDER_LINE_WIDTH / 2, 0));
            _sliderBar.setPosition (sf::Vector2f(_boundingBox.left, _boundingBox.top) + sf::Vector2f(0, _boundingBox.height - barHeight - SLIDER_BAR_HEIGHT / 2));
            _text.setPosition      (sf::Vector2f(_boundingBox.left, _boundingBox.top) + sf::Vector2f(_boundingBox.width / 2 - Utils::textWidth(_text) / 2, _boundingBox.height + SLIDER_BAR_HEIGHT));

            w.draw(_sliderLine);
            w.draw(_sliderBar);
            w.draw(_text);
        }

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) {
            if (_boundingBox.contains(sf::Vector2i(e.x, e.y))) {
                switch(e.button) {
                    case sf::Mouse::Button::Left:
                        _capturedMouse = true;
                        sf::Event::MouseMoveEvent moveEvent;
                        moveEvent.x = e.x;
                        moveEvent.y = e.y;
                        handleMouseMoved(moveEvent); // a little cheaty but it has the code needed to move the bar
                        return true;
                    default:
                        return false;
                }
            } else {
                return false;
            }
        }

        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) {
            if (_capturedMouse && e.button == sf::Mouse::Button::Left) {
                _capturedMouse = false;
                return true;
            }
            return false;
        }

        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) {
            if (_capturedMouse) {

                float scale = (_max - _min) / float(_boundingBox.height);
                int distFromBottom = ((_boundingBox.top + _boundingBox.height) - e.y);

                T valueFromMouse =  distFromBottom * scale + _min;

                if (valueFromMouse < _min) valueFromMouse = _min;
                if (valueFromMouse > _max) valueFromMouse = _max;

                _value = valueFromMouse;
                return true;
            }
            return false;
        }
};

#endif
