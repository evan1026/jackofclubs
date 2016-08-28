#ifndef COLOR_SELECTOR_MENU_H
#define COLOR_SELECTOR_MENU_H

#include "Graphics/Menu/Menu.h"
#include "Graphics/Components/Slider.h"

class ColorSelectorMenu : public Menu {

    // Reason for two copies: making a signed version makes it easier
    sf::Uint8& _redRef;
    sf::Uint8& _greenRef;
    sf::Uint8& _blueRef;

    int _red;
    int _green;
    int _blue;

    Slider<int> _redSlider;
    Slider<int> _greenSlider;
    Slider<int> _blueSlider;

    sf::RectangleShape _colorRect;

    sf::Text _redText;
    sf::Text _greenText;
    sf::Text _blueText;
    sf::Text _titleText;

    protected:
        void renderMenu(sf::RenderWindow& w) override;

    public:
        ColorSelectorMenu(sf::Color& color);

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;
        bool handleKeyPressed(const sf::Event::KeyEvent& e) override;
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
};

#endif
