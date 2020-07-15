#pragma once

#include <type_traits>

#include "Graphics/Components/Component.h"
#include "Graphics/Components/Rectangle.h"
#include "Graphics/Components/Text.h"
#include "Utils/Events/IMouseEventHandler.h"
#include "Utils/Font.h"
#include "Utils/Utils.h"

#define SLIDER_LINE_WIDTH 4
#define SLIDER_BAR_HEIGHT 10

/*!
 * This is a templated class that creates a vertical slider which will
 * directly manipulate some numerical value. Just give the class the
 * variable you want it to control, and it will adjust the value as
 * the user moves the slider up and down.
 *
 * WARNING: If this class has a longer lifetime than the data it
 * manipulates, undefined behavior can occur. So don't let that happen ;)
 */
template<typename T>
class Slider : public Component {
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Slider can only be contructed with number types!");

    std::shared_ptr<Rectangle> _sliderLine;
    std::shared_ptr<Rectangle> _sliderBar;
    std::shared_ptr<Text> _text;

    T _min;
    T _max;

    T& _value;

    bool _capturedMouse = false;

    public:

        /*! \callergraph
         *
         * Constructs a new slider.
         * Mostly just sets up the graphical components and makes note of what data it will be modifying,
         * and what range it can be in.
         *
         * \p min       - The lowest value that will be accepted. This will be the value when the slider is at the lowest point.      <br>
         * \p max       - The largest value that will be accepted. This will be the value when the slider is at the highest point.    <br>
         * \p value     - The data that the slider will modify.                                                                       <br>
         * \p localPos  - The slider's position relative to its parent.                                                               <br>
         * \p size      - The length and width of the bounding box surrounding the slider.                                            <br>
         */
        Slider(T min, T max, T& value, const sf::Vector2i& localPos, const sf::Vector2i& size) :
            Component(localPos, size, false /*childrenAllowed*/),
            _sliderLine(std::make_shared<Rectangle>(sf::Vector2i(SLIDER_LINE_WIDTH, size.y))),
            _sliderBar(std::make_shared<Rectangle>(sf::Vector2i(size.x, SLIDER_BAR_HEIGHT))),
            _text(std::make_shared<Text>(sf::Vector2i(0,0), Utils::toString(value))),
            _min(min),
            _max(max),
            _value(value)
        {
            forceAdd(_sliderLine);
            forceAdd(_sliderBar);
            forceAdd(_text);

            _sliderLine->setFillColor(sf::Color(200, 200, 200));
            _sliderBar->setFillColor(sf::Color::White);

            // Next two loops make sure the text is small enough to fit
            // We just assume that if the min and max fit, that should be small enough,
            // however there are notable cases where that might not apply.
            // Fix this if the issue arises.
            int charSize = _text->getFontSize();

            if (std::is_same<T, sf::Uint8>::value) _text->setString(Utils::toString((int)_max));
            else                                   _text->setString(Utils::toString(_max));

            _text->setFontSize(charSize);
            while (_text->textWidth() > _sliderBar->getSize().x) {
                charSize--;
                _text->setFontSize(charSize);
            }

            if (std::is_same<T, sf::Uint8>::value) _text->setString(Utils::toString((int)_min));
            else                                   _text->setString(Utils::toString(_min));

            _text->setFontSize(charSize);
            while (_text->textWidth() > _sliderBar->getSize().x) {
                charSize--;
                _text->setFontSize(charSize);
            }
        }

        virtual void layout(const sf::RenderWindow& w) override {
            Component::layout(w);

            if (std::is_same<T, sf::Uint8>::value) _text->setString(Utils::toString((int)_value));
            else                                   _text->setString(Utils::toString(_value));

            auto boundingBox = getBounds();

            float barHeight = float(_value - _min) / (_max - _min) * boundingBox.height;

            _sliderLine->setLocalPosition(sf::Vector2i(boundingBox.width / 2 - SLIDER_LINE_WIDTH / 2, 0)); //Centers the line horizontally
            _sliderBar->setLocalPosition (sf::Vector2i(0, boundingBox.height - barHeight - SLIDER_BAR_HEIGHT / 2)); //Centers the bar on the value
            _text->setLocalPosition(sf::Vector2i(boundingBox.width / 2 - _text->textWidth() / 2, boundingBox.height + SLIDER_BAR_HEIGHT)); //Centers the text horizontally at the bottom
        }

        /*! \callergraph
         *
         * Marks that the mouse was clicked on the bar and moving it should
         * also move the bar.
         *
         * Also moves the bar to the current mouse position, if the user clicked
         * on the slider but not on the bar.
         *
         * \p e - The mouse button event representing the click
         */
        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override {
            if (getBounds().contains(sf::Vector2i(e.x, e.y))) {
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

        /*! \callergraph
         *
         * Marks that we are no longer tracking the mouse's movement
         *
         * \p e - The mouse button event representing the button release
         */
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override {
            if (_capturedMouse && e.button == sf::Mouse::Button::Left) {
                _capturedMouse = false;
                return true;
            }
            return false;
        }

        /*! \callergraph
         *
         * Sets the data based on the mouse's position relative to
         * the top and bottom of the bar.
         *
         * Doesn't actually move the bar. The bar's position is calculated
         * later based on the data.
         */
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override {
            if (_capturedMouse) {
                auto boundingBox = getBounds();
                if (e.y < getBounds().top) {
                    _value = _max;
                } else if (e.y > boundingBox.top + boundingBox.height) {
                    _value = _min;
                } else {
                    float scale = (_max - _min) / float(boundingBox.height);
                    int distFromBottom = ((boundingBox.top + boundingBox.height) - e.y);

                    T valueFromMouse =  distFromBottom * scale + _min;

                    if (valueFromMouse < _min) valueFromMouse = _min;
                    if (valueFromMouse > _max) valueFromMouse = _max;

                    _value = valueFromMouse;
                }
                return true;
            }
            return false;
        }
};
